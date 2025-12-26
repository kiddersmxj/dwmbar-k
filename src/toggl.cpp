#include "../inc/toggl.hpp"

static inline std::string trim(const std::string& s) {
    const char* ws = " \t\r\n";
    size_t start = s.find_first_not_of(ws);
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(ws);
    return s.substr(start, end - start + 1);
}

static long parseDurationToSeconds(const std::string& hhmmss) {
    int h = 0, m = 0, s = 0;
    char c1 = 0, c2 = 0;

    std::istringstream iss(hhmmss);
    if (!(iss >> h >> c1 >> m >> c2 >> s)) {
        return 0; // fallback
    }
    if (c1 != ':' || c2 != ':') {
        return 0;
    }
    return static_cast<long>(h) * 3600L +
           static_cast<long>(m) * 60L +
           static_cast<long>(s);
}

static std::string formatSecondsToDuration(long sec) {
    if (sec < 0) sec = 0;

    long h = sec / 3600L;
    long m = (sec % 3600L) / 60L;
    long s = sec % 60L;

    char buf[16];
    std::snprintf(buf, sizeof(buf), "%02ld:%02ld:%02ld", h, m, s);
    return std::string(buf);
}

bool TogglModule::IsRunning(TogglStatus& status) {
    return status.status == "running";
}

TogglModule::TogglStatus TogglModule::GetStatus() {
    std::string Raw;
    std::string Cmd = "/usr/bin/toggl.sh";

    k::ExecCmd(Cmd, Raw);

    TogglStatus ts;

    // If Toggl isn’t running anything, script prints:
    //   "No current Toggl timer."
    if (Raw.find("No current Toggl timer") != std::string::npos) {
        // std::cerr << "Toggl: " << Raw << std::endl;
        ts.status = "none";
        return ts;
    }

    // Expected output format
    // ID:        4189906138
    // Status:    running
    // Duration:  00:31:21
    // Project:   Humb Technologies
    // Desc:      Notion
    // Tags:      Organising

    std::unordered_map<std::string, std::string> fields;

    std::istringstream iss(Raw);
    std::string line;
    while (std::getline(iss, line)) {
        auto colon = line.find(':');
        if (colon == std::string::npos) continue;

        std::string key = trim(line.substr(0, colon));
        std::string val = trim(line.substr(colon + 1));

        fields[key] = val;
    }

    // Assign into struct
    ts.id          = fields["ID"];
    ts.status      = fields["Status"];
    ts.duration    = fields["Duration"];
    ts.project     = fields["Project"];
    ts.description = fields["Desc"];
    ts.tags        = fields["Tags"];

    ts.durationSeconds = parseDurationToSeconds(ts.duration);
    ts.fetchedAt       = std::chrono::system_clock::now();

    return ts;
}

std::string TogglModule::GetDuration(TogglStatus& status) {
    // If not running, just return the last known duration (or empty)
    if (status.status != "running") {
        return status.duration;
    }

    // Running: compute how long it has been since we fetched the status
    auto now   = std::chrono::system_clock::now();
    auto delta = std::chrono::duration_cast<std::chrono::seconds>(now - status.fetchedAt);

    long totalSeconds = status.durationSeconds + delta.count();
    return formatSecondsToDuration(totalSeconds);
}

void TogglModule::run() {
    std::string output = "";
    TogglStatus Raw;
    int RunGetStatus = TogglGetStatusWait;

    while (true) {
        if(RunGetStatus == TogglGetStatusWait) {
            Raw = GetStatus();
            RunGetStatus = 0;
        }
        if(IsRunning(Raw)) {
            output =  TglCol[0] + IToggl + " " + GetDuration(Raw) + " " + TglCol[1] + Raw.tags + " " + \
                      TglCol[2] + Raw.description;
        } else {
            output = NoOutputCode;
        }
        updateOutput(output);
        std::this_thread::sleep_for(std::chrono::milliseconds(TogglSleepTime));
        RunGetStatus++;
    }
}

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

