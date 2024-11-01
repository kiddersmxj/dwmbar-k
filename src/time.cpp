#include "../inc/time.hpp"
#include <chrono>
#include <thread>
#include <iostream>

using namespace std::chrono_literals;

void TimeModule::run() {
    std::string Out = " ";
    std::chrono::system_clock::time_point T = std::chrono::system_clock::now() + 1s;
    std::chrono::system_clock::duration Delay = 990ms;
    std::vector<std::string> S = GetDate();
    std::chrono::system_clock::time_point LastGetDateT = T;
    std::chrono::system_clock::duration LastGetDateDelay = 5min;

    while(1) {
        std::chrono::system_clock::duration t = std::chrono::system_clock::now() - T;
        std::vector<std::string> Output;
        if((Delay - t) > 0s)
            std::this_thread::sleep_for(Delay - t);
        T = std::chrono::system_clock::now();

        Out = R"($(printf ")" + TCol[0] + IDate + " " + TCol[1] + S.at(0) + " " + TCol[2] + S.at(1) + " " + TCol[3] + GetTime() + BDCol + R"("))";

        updateOutput(Out);

        std::chrono::system_clock::duration LastGetDatet = std::chrono::system_clock::now() - LastGetDateT;
        if((LastGetDateDelay - LastGetDatet) < 0s) {
            LastGetDateT = std::chrono::system_clock::now();
            S = GetDate();
        }

    }
}

std::vector<std::string> TimeModule::GetDate() {
    struct tm tstruct;
    time_t now = time(0);
    char date[50];
    tstruct = *localtime(&now);
    strftime(date, sizeof(date), "%d-%m-%Y", &tstruct);

    std::string _day = GetDay(date);
    return {_day, date};
}

std::string TimeModule::GetTime() {
    struct tm tstruct;
    time_t now = time(0);
    tstruct = *localtime(&now);
    char time[50];
    strftime(time, sizeof(time), "%X", &tstruct);
    return time;
}

std::string TimeModule::GetDay(std::string day) {
	int _day = stoi(day.substr(0,2));
	int month = stoi(day.substr(3,2));
	int year = stoi(day.substr(6,4));

    std::vector<int> days={31,28,31,30,31,30,31,31,30,31,30,31};        
    std::vector<std::string> dates={"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};        

    int d = _day;        
    for(int i = 0; i < month-1; i++) d += days[i];        
    if ( ( ( year%4 == 0 && year%100 != 0) || year%400 == 0) && month > 2) d++;        
        for(int i = 1971; i < year; i++) {
            if ( ( i%4 == 0 && i%100 != 0) || i%400 == 0) d += 366;
            else d += 365;
        }
        
    std::string result = dates[ ( d%7 + 3 ) % 7];
    return result;
}

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

