#include "../inc/brightness.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>

double BrightnessModule::getBrightnessPercentage() {
    std::string brightnessPath = "/sys/class/backlight/intel_backlight/brightness";
    std::string maxBrightnessPath = "/sys/class/backlight/intel_backlight/max_brightness";

    std::ifstream brightnessFile(brightnessPath);
    std::ifstream maxBrightnessFile(maxBrightnessPath);

    int currentBrightness = 0;
    int maxBrightness = 0;

    if (brightnessFile.is_open()) {
        brightnessFile >> currentBrightness;
        brightnessFile.close();
    } else {
        std::cerr << "Unable to open file: " << brightnessPath << std::endl;
        return -1.0; // Indicate an error
    }

    if (maxBrightnessFile.is_open()) {
        maxBrightnessFile >> maxBrightness;
        maxBrightnessFile.close();
    } else {
        std::cerr << "Unable to open file: " << maxBrightnessPath << std::endl;
        return -1.0; // Indicate an error
    }

    if (maxBrightness != 0) {
        return (static_cast<double>(currentBrightness) / maxBrightness) * 100;
    } else {
        std::cerr << "Error: max brightness value is zero or not found." << std::endl;
        return -1.0; // Indicate an error
    }
}

void BrightnessModule::run() {
    while (true) {
        double brightnessPercentage = getBrightnessPercentage();
        if (brightnessPercentage >= 0) {
            std::string output = BrCol[0] + ILight + " " + BrCol[1] \
                                 + std::to_string(static_cast<int>(brightnessPercentage)) \
                                 + "%";
            updateOutput(output);
            std::cout << output << std::endl; // Debug print
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(BrightnessSleepTime));
    }
}

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

