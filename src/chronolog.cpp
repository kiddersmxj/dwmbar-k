#include "../inc/chronolog.hpp"

// Function to convert std::string to bool
bool ChronologModule::StringToBool(const std::string &input) {
    // Create a copy of the input and convert it to lowercase.
    std::string normalized(input);
    std::transform(normalized.begin(), normalized.end(), normalized.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    // Define criteria for true values.
    if (normalized == "true" || normalized == "1" || normalized == "yes" || normalized == "on") {
        return true;
    }

    // Define criteria for false values.
    if (normalized == "false" || normalized == "0" || normalized == "no" || normalized == "off") {
        return false;
    }

    // Log detailed error and return false
    std::cerr << "ChronologModule::StringToBool() - Failed to convert input:\n"
              << "  Original input: '" << input << "'\n"
              << "  Normalized input: '" << normalized << "'\n"
              << "  Accepted values: true/1/yes/on or false/0/no/off\n";
    return false;
}

bool ChronologModule::IsRunning(std::string Name) {
    std::string Out = "";
    std::string Cmd = "chronolog -n " + Name + " -o -p";

    int ExitStatus = k::ExecCmd(Cmd, Out);
    if(ExitStatus) {
        return false;
    }

    Out = k::StripTrailingNL(Out);
    auto O = StringToBool(Out);
    return O;
}

std::string ChronologModule::GetElapsed(std::string Name) {
    std::string Out = "";

    k::ExecCmd("chronolog -p -n " + Name, Out);

    return Out;
}

void ChronologModule::run() {
    std::string output = "";
    std::string Name = "bulkit";

    while (true) {
        bool isRunning = IsRunning(Name);
        if(isRunning) {
            output =  CLCol[0] + Name + " " + CLCol[1] + k::StripTrailingNL(GetElapsed(Name)) + BDCol;
            Cooldown = ChronologCooldown;
        } else {
            if(Cooldown > 0) {
                output =  CLCol[0] + Name + " " + CLCol[1] + k::StripTrailingNL(GetElapsed(Name)) + BDCol;
                Cooldown--;
            } else {
                output = "";
            }
        }

        updateOutput(output);
        std::this_thread::sleep_for(std::chrono::milliseconds(ChronologSleepTime));
    }
}

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

