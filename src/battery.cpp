#include "../inc/battery.hpp"
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

const std::string ChargingStatCmd = R"(acpi -b | awk -F'[,:] ' '{print $2}')";
const std::string BatteryLevelCmd = R"(acpi -b | awk -F'[,:%]' '{print $3}')";

std::string BatteryModule::TrimWhiteSpace(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

std::string BatteryModule::TrimNewLine(const std::string& str) {
    size_t first = str.find_first_not_of('\n');
    if (std::string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of('\n');
    return str.substr(first, (last - first + 1));
}

int BatteryModule::Battery() {
    std::string BColI;
    std::string ChargingStat;
    k::ExecCmd(ChargingStatCmd, ChargingStat);
    std::string BatteryLevelString;
    k::ExecCmd(BatteryLevelCmd, BatteryLevelString);

    if (BatteryLevelString.empty()) {
        std::cout << "acpi did not exec" << std::endl;
        return 1;
    }

    ChargingStat = TrimNewLine(TrimWhiteSpace(ChargingStat));
    int BatteryLevel = std::stoi(TrimNewLine(TrimWhiteSpace(BatteryLevelString)));

    if (ChargingStat.empty()) {
        std::cout << "acpi did not exec" << std::endl;
        return 1;
    } else if (ChargingStat == "Full") {
        BColI = BCol[0];
        Charging = 0;
    } else if (ChargingStat == "Not charging") {
        if (BatteryLevel >= BFull) {
            BColI = BCol[0];
            Charging = 0;
        } else if (Charging == 1 || Charging == 4) {
            BColI = BCol[2];
            Charging = 4;
        } else {
            BColI = BCol[5];
            Charging = 1;
        }
    } else if (ChargingStat == "Charging") {
        if (Charging == 2 || Charging == 5) {
            BColI = BCol[1];
            Charging = 5;
        } else {
            BColI = BCol[5];
            Charging = 2;
        }
    } else if (ChargingStat == "Discharging") {
        CIcon.clear();
        Charging = 3;
    } else {
        std::cout << "error cmd: -" << ChargingStat << "- did not match" << std::endl;
    }

    if (Charging != 5) {
        if (BatteryLevel >= BFull) {
            BColI = BCol[0];
            BIcon = IBatteryFull;
        } else if (BatteryLevel >= BThreeQuart) {
            BColI = BCol[1];
            BIcon = IBatteryThreeQuart;
        } else if (BatteryLevel >= BHalf) {
            BColI = BCol[2];
            BIcon = IBatteryHalf;
        } else if (BatteryLevel >= BQuart) {
            BColI = BCol[3];
            BIcon = IBatteryQuart;
        } else if (BatteryLevel >= BEmpty) {
            BColI = BCol[4];
            BIcon = IBatteryEmpty;
        } else if (BatteryLevel == 0) {
            BColI = BCol[5];
            BIcon = IBatteryEmpty;
        } else {
            std::cout << "undefined battery level: -" << std::to_string(BatteryLevel) << "-" << std::endl;
        }
    }

    std::string formattedOutput = R"($(printf ")" + BColI + BIcon + " %s" + CIcon + R"(" ")" \
                                  + BCol[6] + std::to_string(BatteryLevel) + "%" + R"("))";

    updateOutput(formattedOutput);

    return 0;
}

void BatteryModule::run() {
    std::string Out = " ";
    while (true) {
        if (Charging == 5) {
            if (BIcon == IBatteryFull) {
                BIcon = IBatteryEmpty;
            } else if (BIcon == IBatteryThreeQuart) {
                BIcon = IBatteryFull;
            } else if (BIcon == IBatteryHalf) {
                BIcon = IBatteryThreeQuart;
            } else if (BIcon == IBatteryQuart) {
                BIcon = IBatteryHalf;
            } else if (BIcon == IBatteryEmpty) {
                BIcon = IBatteryQuart;
            } else {
                std::cout << "error BIcon cycle for charge: " << BIcon << std::endl;
            }
        }
        Battery();
        std::this_thread::sleep_for(std::chrono::milliseconds(BatterySleepTime));
    }
}

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

