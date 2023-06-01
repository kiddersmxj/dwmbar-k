#include "../include/dwmbar-k.hpp" 

int C = -1;
const std::string ChargingStatCmd = R"(acpi -b | awk -F'[,:] ' '{print $2}')";
const std::string BatteryLevelCmd = R"(acpi -b | awk -F'[,:%]' '{print $3}')";
/*
 * Charging status 0-5
 * ----------------------------------------------------------------------------------
 * | 0 | Full - Charging=Yes, Battery>90% (IBatteryFull, ICharging)
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

int Run() {
    int Clk = PollClock(CDir);
    if(C == Clk) {
        return 0;
    }
    C = Clk;
    if((Clk % BatteryFrq) == 0 || Clk == 0)
        return 1;
    return 0; 
}

int Battery() {
    if(!Run()) {
        return 1;
    }
    std::string BColI;
#ifdef BatDCOUT
    std::cout << "1 " << std::endl;
#endif
    // Get battery info through acpi
    std::string ChargingStat = ExecCmd(ChargingStatCmd, 0, 0);
    std::string BatteryLevelString = ExecCmd(BatteryLevelCmd, 0, 0);
    // Catch errors
    if(BatteryLevelString == "") {
        std::cout << "acpi did not exec" << std::endl;
        return 1;
    }
    // Format outputs
    ChargingStat = TrimNewLine(TrimWhiteSpace(ChargingStat));
    int BatteryLevel = std::stoi(TrimNewLine(TrimWhiteSpace(BatteryLevelString)));

#ifdef BatDCOUT
    std::cout << "CS=" << ChargingStat << std::endl;
#endif
    
    // Catch errors
    if(ChargingStat == "") {
        std::cout << "acpi did not exec" << std::endl;
        return 1;
    } else if(ChargingStat == "Full") {
        // Full only shows when charging (even if 100%)
        // // Therefore show ICharging
        /* CIcon = ICharging; */
        BColI = BCol[0];
        Charging = 0;
    } else if(ChargingStat == "Not charging") {
        if(BatteryLevel >= BFull) {
            BColI = BCol[0];
            Charging = 0;
        } else if(Charging == 1 || Charging == 4) {
            // Flash charging icon to alert - see if annoying
            BColI = BCol[2];
            Charging = 4;
        } else {
            BColI = BCol[5];
            Charging = 1;
        }
    } else if(ChargingStat == "Charging") {
        /* CIcon = ICharging; */
        if(Charging == 2 || Charging == 5) {
            BColI = BCol[1];
            Charging = 5;
        } else {
            BColI = BCol[5];
            Charging = 2;
        }
        // Cycle Icons to show charging
    } else if(ChargingStat == "Discharging") {
        /* std::cout << "DISCHARGE"  << std::endl; */
        CIcon = "";
        Charging = 3;
    } else {
        std::cout << "error cmd: -" << ChargingStat << "- did not match" << std::endl;
    }

    if(Charging != 5) {
        if(BatteryLevel >= BFull) {
            BColI = BCol[0];
            BIcon = IBatteryFull;
        } else if(BatteryLevel >= BThreeQuart) {
            BColI = BCol[1];
            BIcon = IBatteryThreeQuart;
        } else if(BatteryLevel >= BHalf) {
            BColI = BCol[2];
            BIcon = IBatteryHalf;
        } else if(BatteryLevel >= BQuart) {
            BColI = BCol[3];
            BIcon = IBatteryQuart;
        } else if(BatteryLevel >= BEmpty) {
            BColI = BCol[4];
            BIcon = IBatteryEmpty;
        } else if(BatteryLevel == 0) {
            BColI = BCol[5];
            BIcon = IBatteryEmpty;
            /* BIcon = "E"; */
        } else {
            std::cout << "undefined battery level: -" << std::to_string(BatteryLevel) << "-" << std::endl;
        }
    }
    
    /* if(Charging == 4) { */
    /*     if(CIcon == ICharging) { */
    /*         CIcon = ""; */
    /*     } else if(CIcon == "") { */
    /*         CIcon = ICharging; */
    /*     } else { */
    /*         std::cout << "error CIcon cycle for charge: " << CIcon << std::endl; */
    /*     } */
    /* } */

    std::vector<std::string> Output;
#ifdef BatMCOUT
    std::cout << BCol << Std::endl;
#endif
    Output.push_back(R"($(printf ")" + BColI + BIcon + " %s" + CIcon + R"(" ")" + BCol[6] + std::to_string(BatteryLevel) + "%" + R"("))");
    WriteFileLines(Output, BatteryOutputFile);

#ifdef BatMCOUT
    std::cout << Output.front() << std::endl;
#endif
    BreakPoint();

    return 0;
}

int main() {
    while(1) {
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
    }
        Battery();
        std::this_thread::sleep_for(std::chrono::milliseconds(SleepTime));
    }
    return 0;
}
