#include "../inc/claude.hpp"

#include <chrono>
#include <sstream>
#include <thread>

ClaudeModule::ClaudeStatus ClaudeModule::GetStatus() {
    ClaudeStatus s;

    static const char *Cmd =
        "sh -c '"
        "TOKEN=$(jq -r \".claudeAiOauth.accessToken\" "
                "~/.claude/.credentials.json 2>/dev/null);"
        "[ -z \"$TOKEN\" ] || [ \"$TOKEN\" = \"null\" ] && exit 1;"
        "USAGE=$(curl -sf "
                "-H \"Authorization: Bearer $TOKEN\" "
                "-H \"anthropic-beta: oauth-2025-04-20\" "
                "https://api.anthropic.com/api/oauth/usage);"
        "[ -z \"$USAGE\" ] && exit 1;"
        "FH=$(echo \"$USAGE\"    | jq -r \".five_hour.utilization // \\\"?\\\"\");"
        "SD=$(echo \"$USAGE\"    | jq -r \".seven_day.utilization // \\\"?\\\"\");"
        "RESET=$(echo \"$USAGE\" | jq -r \".five_hour.resets_at // \\\"\\\"\" "
                "| xargs -I{} date -d {} +\"%H:%M\" 2>/dev/null);"
        "echo \"$FH $SD $RESET\""
        "'";

    std::string raw;
    k::ExecCmd(Cmd, raw);
    raw = k::StripTrailingNL(raw);
    if (raw.empty()) return s;

    std::istringstream iss(raw);
    std::string fh, sd, reset;
    if (!(iss >> fh >> sd)) return s;
    iss >> reset;  // may be empty if date conversion failed

    try {
        s.fiveHour = std::stoi(fh);
        s.sevenDay = std::stoi(sd);
    } catch (...) {
        return s;
    }

    s.resetTime = reset;
    s.valid     = true;
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

        if (!status.valid) {
            updateOutput(NoOutputCode);
        } else {
            int tierIdx = 0;
            if      (status.fiveHour >= ClaudeFiveHourRed)    tierIdx = 2;
            else if (status.fiveHour >= ClaudeFiveHourYellow) tierIdx = 1;

            const std::string& tier  = ClTier[tierIdx];
            const std::string& gauge = ClGauge[tierIdx];

            std::string out =
                tier     + gauge   + " " +
                ClCol[1] + std::to_string(status.fiveHour) + "%/" +
                           std::to_string(status.sevenDay) + "% " +
                tier     + IReset  + " " +
                ClCol[3] + status.resetTime;

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
