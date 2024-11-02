#include "../inc/cpu.hpp"
#include <iostream>
#include <thread>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>

// Function to parse CPU statistics from /proc/stat
std::vector<long> ParseCpuStats() {
    std::ifstream file("/proc/stat");
    std::string line;
    std::vector<long> cpuStats;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            if (line.compare(0, 3, "cpu") == 0) {
                std::istringstream ss(line);
                std::string label;
                ss >> label; // Skip the "cpu" label

                long value;
                while (ss >> value) {
                    cpuStats.push_back(value);
                }
                break;
            }
        }
        file.close();
    }
    return cpuStats;
}

// Function to calculate CPU usage percentage
double CPUModule::GetCpuUsage() {
    double usage(0);

    static std::vector<long> prevCpuStats = ParseCpuStats();
    std::vector<long> currCpuStats = ParseCpuStats();

    long prevIdle = prevCpuStats[3] + prevCpuStats[4];
    long currIdle = currCpuStats[3] + currCpuStats[4];

    long prevTotal = 0;
    long currTotal = 0;
    for (size_t i = 0; i < prevCpuStats.size(); i++) {
        prevTotal += prevCpuStats[i];
        currTotal += currCpuStats[i];
    }

    long totalDiff = currTotal - prevTotal;
    long idleDiff = currIdle - prevIdle;

    prevCpuStats = currCpuStats;

    usage = (totalDiff - idleDiff) * 100.0 / totalDiff;
    return usage;
}

void CPUModule::run() {
    while (true) {
        double usage = GetCpuUsage();
        usage = static_cast<int>(usage + 0.5);
        std::string output = CPUCol[0] + ICPU + " " + CPUCol[1] \
                             + std::to_string(static_cast<int>(usage + 0.5)) + "%";
        updateOutput(output);
        std::this_thread::sleep_for(std::chrono::milliseconds(CPUSleepTime));
    }
}

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

