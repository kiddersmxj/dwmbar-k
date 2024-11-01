#include "../inc/weather.hpp"
#include <iostream>
#include <chrono>
#include <thread>

void WeatherModule::run() {
    std::string Out = " ";
    while (true) {
        Out = GetWeather();
        updateOutput(Out);

        std::this_thread::sleep_for(std::chrono::milliseconds(WeatherSleepTime));
    }
}

std::string WeatherModule::GetWeather() {
    std::string WttrHex;
    k::ExecCmd(R"(timeout 1 curl -s "wttr.in/$city?format=1&m" | hexdump -v -e '"\\\x" 1/1 "%02x"')", WttrHex);
    if (WttrHex.empty()) return "NaN";

    char* HexArray = new char[WttrHex.length() + 1];
    strcpy(HexArray, WttrHex.c_str());
    char* substr = strtok(HexArray, R"(\)");

    int i = 0;
    int space = 0;
    std::string HexOutput = "";
    while(substr != NULL) {
        if(strcmp(substr, "x0a") == 0) {
        } else if(i < 4 && strcmp(substr, "xef") != 0) {
            HexOutput += "\\";
            HexOutput += substr;
        } else if(strcmp(substr, "x2b") == 0) {
        } else if(strcmp(substr, "x20") == 0 && space == 0) {
            space++;
        } else if(space == 1) {
            HexOutput += "\\";
            HexOutput += substr;
        }
        substr = strtok(NULL, R"(\)");
        i++;
    }
    delete[] HexArray;

    HexOutput = R"($(printf ")" + WCol[0] + HexOutput + BDCol + R"("))";

    return HexOutput;
}

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

