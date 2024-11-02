#include "../inc/bluetooth.hpp"

#include <vector>
#include <chrono>
#include <thread>

void BluetoothModule::run() {
    std::string output = " ";
    while (true) {
        std::string deviceName = getBluetoothDeviceName();

        if (!deviceName.empty()) {
            output = BlCol[0] + IBluetooth + " " + BlCol[1] + deviceName;
            // std::cout << "BluetoothModule: Updating output to " << output << std::endl;  // Debug
            updateOutput(output);
        } else {
            // std::cout << "BluetoothModule: No Bluetooth" << std::endl;  // Debug
            updateOutput(NoOutputCode);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(BluetoothSleepTime));
    }
}

std::string BluetoothModule::getBluetoothDeviceName() {
    std::string device;
    k::ExecCmd("bluetoothctl info | grep Name | awk -F': ' '{print $2}'", device);
    return k::StripTrailingNL(device);
}

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

