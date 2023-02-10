#include <iostream>
#include <ctime>
#include <cstring>
#include "../include/dwmbar-k.hpp" 

int C = -1;

int Run() {
    if(C == PollClock(CDir)) {
        return 0;
    }
    C = PollClock(CDir);
    if((PollClock(CDir) % TimeFrq) == 0 || PollClock(CDir) == 0)
        return 1;
    return 0; 
}

std::string GetDay(std::string day) {
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

// Get current date/time, format is dd-mm-YYYY hh:mm:ss
std::string GetDateAndTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       date[50];
    char       time[50];
    tstruct = *localtime(&now);

    strftime(date, sizeof(date), "%d-%m-%Y", &tstruct);
    std::string _day = GetDay(date);
    strftime(time, sizeof(time), "%X", &tstruct);

    std::string status = _day + " " + date + " " + time;
    return status;
}

int Time() {
    if(!Run()) {
        return 1;
    }

    std::vector<std::string> Output;
    Output.push_back(GetDateAndTime());
    WriteFileLines(Output, TimeOutputFile);

#ifdef MCOUT
    std::cout << Output.front() << std::endl;
    BreakPoint();
#endif
    return 0;
}

int main() {
    while(1) {
        Time();
        std::this_thread::sleep_for(std::chrono::milliseconds(SleepTime));
    }
    return 0;
}
