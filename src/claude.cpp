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
// Emits one line — "<ok|bad> <polled-epoch> <5h%> <7d%> <polled-HH:MM>
// <reset-HH:MM>" — or nothing at all when there is no reading to have.
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
    (if (.last_poll.outcome // "") == "ok" then "ok" else "bad" end)
    + " " + (if (.latest.polled_at // "") == "" then "-"
             else .latest.polled_at end)
    + " " + ((.latest.five_hour // 0) | round | tostring)
    + " " + ((.latest.seven_day // 0) | round | tostring)
    + " " + (if (.latest.five_hour_resets_at // "") == "" then "-"
             else .latest.five_hour_resets_at end)
  end' 2>/dev/null)
[ $# -ge 5 ] || exit 1
PE=0; PH=-; RH=-
# Guard the placeholder: `date -d -` succeeds and invents a time.
[ "$2" = "-" ] || { PE=$(date -d "$2" +%s 2>/dev/null); PH=$(date -d "$2" +%H:%M 2>/dev/null); }
[ "$5" = "-" ] || RH=$(date -d "$5" +%H:%M 2>/dev/null)
echo "$1 ${PE:-0} $3 $4 ${PH:--} ${RH:--}"
)";

    std::string raw;
    k::ExecCmd(Cmd, raw);
    raw = k::StripTrailingNL(raw);
    if (raw.empty()) return s;  // unreachable, or bursar has never polled

    std::istringstream iss(raw);
    std::string outcome, polledEpoch, fh, sd, polledHM, resetHM;
    if (!(iss >> outcome >> polledEpoch >> fh >> sd >> polledHM >> resetHM))
        return s;

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

    while (true) {
        if (RunGetStatus >= ClaudeGetStatusWait) {
            status = GetStatus();
            RunGetStatus = 0;
        }

        if (status.state == State::NoReading) {
            // A dash, not a zero: 0% reads as good news, and the truth here is
            // that we do not know.
            updateOutput(ClCol[5] + ClGauge[0] + " " +
                         ClCol[5] + ClaudeNoReading);
        } else {
            int tierIdx = 0;
            if      (status.fiveHour >= ClaudeFiveHourRed)    tierIdx = 2;
            else if (status.fiveHour >= ClaudeFiveHourYellow) tierIdx = 1;

            const bool stale = (status.state == State::Stale);
            // Stale figures never wear the tier colour — that would present an
            // old number as a live one. The gauge keeps its tier *shape*.
            const std::string& tier  = stale ? ClCol[4] : ClTier[tierIdx];
            const std::string& gauge = ClGauge[tierIdx];

            // Trailing field: when current, the time the 5-hour window resets;
            // when stale, the time this figure was actually taken.
            const std::string& trailIcon = stale ? IStale : IReset;
            const std::string& trailTime =
                stale ? status.polledTime : status.resetTime;

            std::string out =
                tier     + gauge   + " " +
                ClCol[1] + std::to_string(status.fiveHour) + "%/" +
                           std::to_string(status.sevenDay) + "% " +
                tier     + trailIcon;

            if (!trailTime.empty())
                out += " " + ClCol[3] + trailTime;

            updateOutput(out);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(ClaudeSleepTime));
        RunGetStatus++;
    }
}

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
