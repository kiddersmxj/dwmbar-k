#include <iostream>
#include <cstring>
#include "../include/dwmbar-k.h" 

const std::string ChargingStatCmd = R"(acpi -b | awk -F'[,:] ' '{print $2}')";
const std::string BatteryLevelCmd = R"(acpi -b | awk -F'[,:%]' '{print $3}')";
/*
 * Charging status 0-5
 * ----------------------------------------------------------------------------------
 * | 0 | Full - Charging=Yes, Battery=100% (IBatteryFull, ICharging)
 * |---|-----------------------------------------------------------------------------
 * | 1 | Not charging - Charging=No, Battery=~ (Will auto switch to Charging=4)
 * |---|-----------------------------------------------------------------------------
 * | 2 | Charging - Charging=Yes, Battery!=100 (Will auto switch to Charging=5)
 * |---|-----------------------------------------------------------------------------
 * | 3 | Discharging - Charging=No, Battery=~ (Checks battery level to choose BIcon)
 * |---|-----------------------------------------------------------------------------
 * | 4 | NOT CHARGING - (Will flash CIcon)
 * |---|-----------------------------------------------------------------------------
 * | 5 | CHARGING - (Will animate BIcon)
 * ---------------------------------------------------------------------------------- */
int Charging = 3;
// Checks if battery has maxxed out to stop not charging warning
// Battery will max out at 100% then not charge until it hits 90% again
int Maxxed = 0;
std::string BIcon = IBatteryEmpty;
std::string CIcon = "";

std::string TrimWhiteSpace(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

std::string TrimNewLine(const std::string& str) {
    size_t first = str.find_first_not_of('\n');
    if (std::string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of('\n');
    return str.substr(first, (last - first + 1));
}

int main() {
    // Adjust to read config file from dwmbar-k
    while(1) {
        // Get battery info through acpi
        std::string ChargingStat = ExecCmd(ChargingStatCmd, 0, 0);
        std::string BatteryLevelString = ExecCmd(BatteryLevelCmd, 0, 0);
        if(BatteryLevelString == "") {
            std::cout << "acpi did not exec" << std::endl;
            return 1;
        }
        // Format outputs
        ChargingStat = TrimNewLine(TrimWhiteSpace(ChargingStat));
        int BatteryLevel = std::stoi(TrimNewLine(TrimWhiteSpace(BatteryLevelString)));
    
        if(ChargingStat == "") {
            std::cout << "acpi did not exec" << std::endl;
            return 1;
        } else if(ChargingStat == "Full") {
            /* std::cout << "full innit" << std::endl; */
            Maxxed = 1;
            CIcon = ICharging;
            Charging = 0;
        } else if(ChargingStat == "Not charging") {
            /* std::cout << ICharging << std::endl; */
            // Flash charging icon to alert - see if annoying
            if(Maxxed) {
                CIcon = ICharging;
                Charging = 0;
            } else if(Charging == 1 || Charging == 4) {
                Charging = 4;
            } else {
                Charging = 1;
            }
        } else if(ChargingStat == "Charging") {
            /* std::cout << ICharging << " CHARGE"  << std::endl; */
            CIcon = ICharging;
            if(Charging == 2 || Charging == 5) {
                Charging = 5;
            } else {
                Charging = 2;
            }
            Maxxed = 0;
            // Cycle Icons to show charging
        } else if(ChargingStat == "Discharging") {
            /* std::cout << "DISCHARGE"  << std::endl; */
            CIcon = "";
            Charging = 3;
            Maxxed = 0;
        } else {
            std::cout << "error cmd: -" << ChargingStat << "- did not match" << std::endl;
        }

        if(Charging == 5) {
            if(BIcon == IBatteryFull) {
                BIcon = IBatteryEmpty;
            } else if(BIcon == IBatteryThreeQuart) {
                BIcon = IBatteryFull;
            } else if(BIcon == IBatteryHalf) {
                BIcon = IBatteryThreeQuart;
            } else if(BIcon == IBatteryQuart) {
                BIcon = IBatteryHalf;
            } else if(BIcon == IBatteryEmpty) {
                BIcon = IBatteryQuart;
            } else {
                std::cout << "error BIcon cycle for charge: " << BIcon << std::endl;
            }
        } else {
            if(BatteryLevel == BFull) {
                BIcon = IBatteryFull;
            } else if(BatteryLevel >= BThreeQuart) {
                BIcon = IBatteryThreeQuart;
            } else if(BatteryLevel >= BHalf) {
                BIcon = IBatteryHalf;
            } else if(BatteryLevel >= BQuart) {
                BIcon = IBatteryQuart;
            } else if(BatteryLevel >= BEmpty) {
                BIcon = IBatteryEmpty;
            } else {
                std::cout << "undefined battery level: " << std::to_string(BatteryLevel) << std::endl;
            }
        }
    
        if(Charging == 4) {
            if(CIcon == ICharging) {
                CIcon = "";
            } else if(CIcon == "") {
                CIcon = ICharging;
            } else {
                std::cout << "error CIcon cycle for charge: " << CIcon << std::endl;
            }
        }
    
        std::cout << BIcon << " " << std::to_string(BatteryLevel) << "% " << CIcon << std::endl;
    
    }
    return 0;
}
