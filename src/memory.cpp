#include "../inc/memory.hpp"

#include <fstream>
#include <cstdio>
#include <string>
#include <chrono>
#include <thread>

void MemoryModule::run() {
    std::string output = " ";
    while (true) {
        Usage u = getUsage();

        if (u.valid) {
            if (u.swap != 0) {
                output = MemCol[0] + IMemory + " " + MemCol[1] +
                         std::to_string(u.mem) + "%/" + std::to_string(u.swap) + "%";
            } else {
                output = MemCol[0] + IMemory + " " + MemCol[1] +
                         std::to_string(u.mem) + "%";
            }
            updateOutput(output);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(MemorySleepTime));
    }
}

// Read /proc/meminfo directly rather than shelling out. The previous version
// ran two `free | awk` pipelines per tick — six forks, ten times a second, to
// produce two integers the kernel already exposes as plain text.
MemoryModule::Usage MemoryModule::getUsage() {
    Usage u;

    std::ifstream f("/proc/meminfo");
    if (!f) return u;

    long memTotal = -1, memAvailable = -1, swapTotal = -1, swapFree = -1;
    std::string line;

    // Lines are "Key:<ws>value kB". Read whole lines rather than tokens: the
    // trailing unit would otherwise desync a >> loop on every field we skip.
    while (std::getline(f, line)) {
        long value;
        if (std::sscanf(line.c_str(), "MemTotal: %ld", &value) == 1)          memTotal     = value;
        else if (std::sscanf(line.c_str(), "MemAvailable: %ld", &value) == 1) memAvailable = value;
        else if (std::sscanf(line.c_str(), "SwapTotal: %ld", &value) == 1)    swapTotal    = value;
        else if (std::sscanf(line.c_str(), "SwapFree: %ld", &value) == 1)     swapFree     = value;
        else continue;

        if (memTotal >= 0 && memAvailable >= 0 && swapTotal >= 0 && swapFree >= 0)
            break;                       // got everything, skip the long tail
    }

    if (memTotal <= 0 || memAvailable < 0) return u;

    long memUsed = memTotal - memAvailable;
    if (memUsed < 0) memUsed = 0;
    u.mem = static_cast<int>((memUsed * 100 + memTotal / 2) / memTotal);

    if (swapTotal > 0 && swapFree >= 0) {
        long swapUsed = swapTotal - swapFree;
        if (swapUsed < 0) swapUsed = 0;
        u.swap = static_cast<int>((swapUsed * 100 + swapTotal / 2) / swapTotal);
    }

    u.valid = true;
    return u;
}



// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 
