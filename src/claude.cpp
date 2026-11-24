#include "../inc/claude.hpp"

#include <algorithm>
#include <chrono>
#include <ctime>
#include <sstream>
#include <thread>

// Read the figures from bursar, which polls Anthropic every five minutes on
// behalf of the whole fleet. This machine deliberately holds no credential and
// never talks to the API itself: several independent pollers share one small
// rate limit and exhaust it for each other.
//
// Emits one line, "<ok|bad> <polled-epoch> <5h%> <7d%> <polled-HH:MM>
// <reset> <5h|7d>", or nothing at all when there is no reading to have.
//
// The last field is which window bursar gates on. Anthropic dropped the
// five-hour window on 2026-10-29; until this read it, the bar showed the
// missing figure as 0%, which looked like good news. With only the week, the
// reset is the week's, as "Tue_16:00" (an underscore so it stays one field).
ClaudeModule::ClaudeStatus ClaudeModule::GetStatus() {
    ClaudeStatus s;

    // The URL comes from the user's own config; strip quotes that would end
    // the shell literal rather than trusting it to contain none.
    std::string url = ClaudeBursarURL;
    url.erase(std::remove(url.begin(), url.end(), '\''), url.end());

    const std::string Cmd =
        "J=$(curl -sf --max-time 5 '" + url + "') || exit 1\n"
        R"(
[ -n "$J" ] || exit 1
set -- $(printf %s "$J" | jq -r '
  if .latest == null then empty else
    (if (.latest.regime // (if .latest.five_hour == null then "weekly"
                            else "five_hour" end)) == "weekly"
     then "7d" else "5h" end) as $w
    | (if $w == "7d" then (.latest.seven_day_resets_at // "")
       else (.latest.five_hour_resets_at // "") end) as $r
    | (if (.last_poll.outcome // "") == "ok" then "ok" else "bad" end)
    + " " + (if (.latest.polled_at // "") == "" then "-"
             else .latest.polled_at end)
    + " " + ((.latest.five_hour // 0) | round | tostring)
    + " " + ((.latest.seven_day // 0) | round | tostring)
    + " " + (if $r == "" then "-" else $r end)
    + " " + $w
  end' 2>/dev/null)
[ $# -ge 6 ] || exit 1
PE=0; PH=-; RH=-
# Guard the placeholder: `date -d -` succeeds and invents a time.
[ "$2" = "-" ] || { PE=$(date -d "$2" +%s 2>/dev/null); PH=$(date -d "$2" +%H:%M 2>/dev/null); }
if [ "$6" = "7d" ]; then F=%a_%H:%M; else F=%H:%M; fi
[ "$5" = "-" ] || RH=$(date -d "$5" +$F 2>/dev/null)
echo "$1 ${PE:-0} $3 $4 ${PH:--} ${RH:--} $6"
)";

    std::string raw;
    k::ExecCmd(Cmd, raw);
    raw = k::StripTrailingNL(raw);
    if (raw.empty()) return s;  // unreachable, or bursar has never polled

    std::istringstream iss(raw);
    std::string outcome, polledEpoch, fh, sd, polledHM, resetHM, window;
    if (!(iss >> outcome >> polledEpoch >> fh >> sd >> polledHM >> resetHM >> window))
        return s;
    s.weekly = (window == "7d");
    std::replace(resetHM.begin(), resetHM.end(), '_', ' ');

    long long polledAt = 0;
    try {
        s.fiveHour = std::stoi(fh);
        s.sevenDay = std::stoi(sd);
        polledAt   = std::stoll(polledEpoch);
    } catch (...) {
        return s;
    }

    s.resetTime  = (resetHM  == "-") ? "" : resetHM;
    s.polledTime = (polledHM == "-") ? "" : polledHM;

    // A figure is only current if bursar's last poll succeeded *and* the
    // reading itself is recent. A failed poll (429, expired credential) still
    // returns the last good figure, which is worth showing but is old; so is a
    // figure whose timestamp has fallen behind, which means the poller stopped.
    const long long age = static_cast<long long>(std::time(nullptr)) - polledAt;
    const bool fresh = (outcome == "ok") && polledAt > 0 &&
                       age < ClaudeStaleSeconds;

    s.state = fresh ? State::Fresh : State::Stale;
    return s;
}

void ClaudeModule::run() {
    ClaudeStatus status;
    int RunGetStatus = ClaudeGetStatusWait;
    int wait = ClaudeGetStatusWait;
    bool retrying = false;

    while (true) {
        if (RunGetStatus >= wait) {
            ClaudeStatus fetched = GetStatus();
            RunGetStatus = 0;
            wait = ClaudeGetStatusWait;

            if (fetched.state != State::NoReading) {
                status = fetched;
                retrying = false;
            } else if (status.state != State::NoReading && !retrying) {
                // One failed fetch is usually a blip on the way to bursar, not
                // bursar losing its figure. Keep the last reading, marked old,
                // and try again soon rather than blanking it for a whole poll.
                status.state = State::Stale;
                retrying = true;
                wait = ClaudeRetryWait;
            } else {
                // Two in a row, or nothing to fall back on: admit we do not
                // know, and wait out a normal poll before asking again.
                status = fetched;
                retrying = false;
            }
        }

        if (status.state == State::NoReading) {
            // A dash, not a zero: 0% reads as good news, and the truth here is
            // that we do not know.
            updateOutput(ClCol[5] + ClGauge[0] + " " +
                         ClCol[5] + ClaudeNoReading + BDCol);
        } else {
            // The tier follows the window bursar gates on. With only the week
            // there is no five-hour figure to colour by, and the week fills
            // over days, so it has thresholds of its own.
            const int pct    = status.weekly ? status.sevenDay : status.fiveHour;
            const int red    = status.weekly ? ClaudeWeeklyRed : ClaudeFiveHourRed;
            const int yellow = status.weekly ? ClaudeWeeklyYellow : ClaudeFiveHourYellow;
            int tierIdx = 0;
            if      (pct >= red)    tierIdx = 2;
            else if (pct >= yellow) tierIdx = 1;

            const bool stale = (status.state == State::Stale);
            // Stale figures never wear the tier colour — that would present an
            // old number as a live one. The gauge keeps its tier *shape*.
            const std::string& tier  = stale ? ClCol[4] : ClTier[tierIdx];
            const std::string& gauge = ClGauge[tierIdx];

            // Trailing field: when current, when the gated window resets;
            // when stale, the time this figure was actually taken.
            const std::string& trailIcon = stale ? IStale : IReset;
            const std::string& trailTime =
                stale ? status.polledTime : status.resetTime;

            std::string out = tier + gauge + " " + ClCol[1] +
                (status.weekly
                     ? "7d " + std::to_string(status.sevenDay) + "%"
                     : std::to_string(status.fiveHour) + "%/" +
                           std::to_string(status.sevenDay) + "%");

            // bursar reports a null five_hour_resets_at while the window sits
            // idle at 0%, so the reset field can legitimately be absent — and a
            // reset icon with no time after it says nothing. The stale icon is
            // the exception: on its own it still means "this figure is old".
            if (!trailTime.empty())
                out += " " + tier + trailIcon + " " + ClCol[3] + trailTime;
            else if (stale)
                out += " " + tier + trailIcon;

            updateOutput(out + BDCol);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(ClaudeSleepTime));
        RunGetStatus++;
    }
}

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
