#include "../inc/throttling.hpp"

#include <iostream>
#include <thread>
#include <fstream>
#include <string>
#include <chrono>

bool ThermalThrottlingModule::CheckThermalThrottling() {
    std::ifstream file("/sys/devices/system/cpu/cpu0/thermal_throttle/package_throttle_count");
    int throttleCount = 0;

    if (file.is_open()) {
        file >> throttleCount;
        file.close();
    } else {
        std::cerr << "Error: Unable to access thermal throttle information." << std::endl;
        return false;
    }

    // std::cout << throttleCount << std::endl;

    return throttleCount > 0;
}

void ThermalThrottlingModule::run() {
    while (true) {
        bool isThrottling = CheckThermalThrottling();
        std::string output = isThrottling ? ThCol[0] + IThermometer \
                             + BDCol : "";
        updateOutput(output);
        // std::cout << output << std::endl; // Debug print
        std::this_thread::sleep_for(std::chrono::milliseconds(ThermalThrottleSleepTime));
    }
}

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

