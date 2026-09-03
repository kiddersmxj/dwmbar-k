#include "../inc/ccorral.hpp"

#include <chrono>
#include <thread>

// Run a command, parse its (single-int) stdout. Returns false on empty
// output (e.g. ccorral not installed) or a non-numeric line.
static bool parseCount(const char *cmd, int &out) {
    std::string raw;
    k::ExecCmd(cmd, raw);
    raw = k::StripTrailingNL(raw);
    if (raw.empty()) return false;
    try {
        out = std::stoi(raw);
    } catch (...) {
        return false;
    }
    return true;
}

CcorralModule::CcorralStatus CcorralModule::GetStatus() {
    CcorralStatus s;

    // Cheap reads (file-backed) — safe to poll every tick. If these fail the
    // CLI is absent/broken, so the whole segment goes invalid.
    if (!parseCount("ccorral count unread 2>/dev/null", s.unread))   return s;
    if (!parseCount("ccorral count open 2>/dev/null", s.open))       return s;
    if (!parseCount("ccorral count running 2>/dev/null", s.running)) return s;

    // `ccorral mute-status` prints "true" when muted (exit 1 when not, so the
    // string, not the exit code, is the signal).
    std::string raw;
    k::ExecCmd("ccorral mute-status 2>/dev/null", raw);
    s.muted = (k::StripTrailingNL(raw) == "true");

    s.valid = true;
    return s;
}

void CcorralModule::run() {
    CcorralStatus status;
    int attention = 0;                      // persists between X11 polls
    int RunAttention = CcorralAttentionWait; // force a read on first iteration

    while (true) {
        status = GetStatus();

        // attention reads X11 (~one xprop per window) — poll less often.
        if (RunAttention >= CcorralAttentionWait) {
            parseCount("ccorral count attention 2>/dev/null", attention);
            RunAttention = 0;
        }
        status.attention = attention;

        if (!status.valid) {
            updateOutput(NoOutputCode);
        } else {
            std::string out =
                CcCol[0] + (status.muted ? ICcorralMuted : ICcorralBell) + " " +
                CcCol[1] + std::to_string(status.unread) + "/" +
                std::to_string(status.attention) + "/" +
                std::to_string(status.running)   + "/" +
                std::to_string(status.open) + BDCol;

            updateOutput(out);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(CcorralSleepTime));
        RunAttention++;
    }
}

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
