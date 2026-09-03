#include "../inc/brightness.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <chrono>
#include <thread>
#include <string>
#include <std-k>

// Fallback brightness reader using brightnessctl
double BrightnessModule::getBrightnessFromBrightnessctl() {
    std::string Output;
    k::ExecCmd("brightnessctl", Output);

    if (Output.empty()) {
        std::cerr << "brightnessctl returned no output" << std::endl;
        return -1.0;
    }

    // Look for "(xx%)"
    std::size_t openParen = Output.find('(');
    std::size_t percent = Output.find('%', openParen);

    if (openParen == std::string::npos || percent == std::string::npos) {
        std::cerr << "Unable to parse brightnessctl output" << std::endl;
        return -1.0;
    }

    std::string numberStr = Output.substr(openParen + 1, percent - openParen - 1);

    try {
        double value = std::stod(numberStr);
        return value;
    } catch (...) {
        std::cerr << "Failed converting brightnessctl percentage" << std::endl;
        return -1.0;
    }
}

// Enumerate /sys/class/backlight rather than assuming a vendor. This used to
// hardcode intel_backlight, so on AMD (amdgpu_bl*) or any other driver the
// sysfs path never resolved and every tick fell through to brightnessctl.
double BrightnessModule::getBrightnessPercentage() {
    namespace fs = std::filesystem;

    std::error_code ec;
    for (const auto& entry : fs::directory_iterator("/sys/class/backlight", ec)) {
        std::ifstream brightnessFile(entry.path() / "brightness");
        std::ifstream maxBrightnessFile(entry.path() / "max_brightness");

        int currentBrightness = 0;
        int maxBrightness = 0;

        if (!(brightnessFile >> currentBrightness)) continue;
        if (!(maxBrightnessFile >> maxBrightness)) continue;
        if (maxBrightness == 0) continue;

        double value = (static_cast<double>(currentBrightness) / maxBrightness) * 100;
        if (value <= 0.0) {
            return getBrightnessFromBrightnessctl();   // genuinely off, or bogus
        }
        return value;
    }

    return getBrightnessFromBrightnessctl();           // no backlight class, or ec set
}

void BrightnessModule::run() {
    while (true) {
        double brightnessPercentage = getBrightnessPercentage();
        if (brightnessPercentage >= 0) {
            std::string output = BrCol[0] + ILight + " " + BrCol[1] \
                                 + std::to_string(static_cast<int>(brightnessPercentage)) \
                                 + "%";
            updateOutput(output);
            // std::cout << output << std::endl; // Debug print
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(BrightnessSleepTime));
    }
}

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

