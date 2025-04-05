#include "../inc/winconnect.hpp"

#include <iostream>
#include <thread>
#include <fstream>
#include <string>
#include <chrono>

bool WinConnectModule::IsWinConnected() {
    std::ifstream file("/home/kidders/.cache/winconnect/winconnect");
    int Connected = 0;

    if (file.is_open()) {
        file >> Connected;
        file.close();
    } else {
        std::cerr << "Error: Unable to access connection information." << std::endl;
        return false;
    }

    return Connected > 0;
}

void WinConnectModule::run() {
    while (true) {
        bool isConnected = IsWinConnected();
        std::string output = isConnected ? WCCol[0] + IHdd \
                             + BDCol : "";
        updateOutput(output);
        // std::cout << "WinConnect: " << output << std::endl; // Debug print
        std::this_thread::sleep_for(std::chrono::milliseconds(WinConnectSleepTime));
    }
}

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

