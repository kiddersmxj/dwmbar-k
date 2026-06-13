#include "../inc/archaudit.hpp"

#include <chrono>
#include <sstream>
#include <thread>

ArchAuditModule::ArchAuditStatus ArchAuditModule::GetStatus() {
    ArchAuditStatus s;

    // arch-audit -q  prints one vulnerable package per line;
    // arch-audit -uq prints only those with a fix available.
    // arch-audit exits 0 even when vulnerabilities are present, so the
    // count of non-empty lines is the signal. The wrapper exits
    // non-empty only on success; a genuine failure (e.g. offline)
    // yields empty stdout → invalid. Output is "<vulns> <upgradable>".
    static const char *Cmd =
        "sh -c '"
        "V=$(arch-audit -q 2>/dev/null) || exit 1;"
        "U=$(arch-audit -uq 2>/dev/null);"
        "VC=$(printf \"%s\\n\" \"$V\" | grep -c \".\");"
        "UC=$(printf \"%s\\n\" \"$U\" | grep -c \".\");"
        "echo \"$VC $UC\""
        "'";

    std::string raw;
    k::ExecCmd(Cmd, raw);
    raw = k::StripTrailingNL(raw);
    if (raw.empty()) return s;

    std::istringstream iss(raw);
    std::string vc, uc;
    if (!(iss >> vc >> uc)) return s;

    try {
        s.count      = std::stoi(vc);
        s.upgradable = std::stoi(uc);
    } catch (...) {
        return s;
    }

    s.valid = true;
    return s;
}

void ArchAuditModule::run() {
    ArchAuditStatus status;
    int RunGetStatus = ArchAuditGetStatusWait;

    while (true) {
        if (RunGetStatus >= ArchAuditGetStatusWait) {
            status = GetStatus();
            RunGetStatus = 0;
        }

        if (!status.valid) {
            updateOutput(NoOutputCode);
        } else {
            int tierIdx = 0;
            if      (status.count >= ArchAuditRed)    tierIdx = 2;
            else if (status.count >= ArchAuditYellow) tierIdx = 1;

            const std::string& tier = ArTier[tierIdx];

            std::string out =
                tier     + IArchAudit + " " +
                ArCol[1] + std::to_string(status.count) + "/" +
                           std::to_string(status.upgradable);

            updateOutput(out);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(ArchAuditSleepTime));
        RunGetStatus++;
    }
}

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
