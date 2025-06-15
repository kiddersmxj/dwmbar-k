#include "../inc/memory.hpp"

#include <vector>
#include <chrono>
#include <thread>

void MemoryModule::run() {
    std::string output = " ";
    while (true) {
        std::string Mem = getMemory();
        std::string Swp = getSwap();
        int S = std::stoi(Swp);

        if (S != 0) {
            output = MemCol[0] + IMemory + " " + MemCol[1] + Mem + "%/" + Swp + "%";
            // std::cout << "MemoryModule: Updating output to " << output << std::endl;  // Debug
            updateOutput(output);
        } else {
            output = MemCol[0] + IMemory + " " + MemCol[1] + Mem + "%";
            // std::cout << "MemoryModule: Updating output to " << output << std::endl;  // Debug
            updateOutput(output);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(MemorySleepTime));
    }
}

std::string MemoryModule::getMemory() {
    std::string mem;
    // Full Value
    // k::ExecCmd(R"(free -m | awk '/^Mem:/ {printf "%.1f/%.1f", $3/1024, $2/1024}')", mem);
    // Percentage
    k::ExecCmd(R"(free | awk '/^Mem:/ {printf "%.0f", $3/$2 * 100}')", mem);
    return k::StripTrailingNL(mem);
}

std::string MemoryModule::getSwap() {
    std::string swp;
    // Full Value
    // k::ExecCmd(R"(free -m | awk '/^Swap:/ {printf "%.1f/%.1f", $3/1024, $2/1024}')", swp);
    // Percentage
    k::ExecCmd(R"(free | awk '/^Swap:/ {if ($2 > 0) printf "%.0f", $3/$2 * 100; else print "0"}')", swp);
    return k::StripTrailingNL(swp);
}



// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

