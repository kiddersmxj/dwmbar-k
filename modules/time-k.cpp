#include <chrono>
#include <ctime>
#include "../include/dwmbar-k.hpp" 

int C = -1;

int Run(std::chrono::system_clock::time_point &T) {
    std::chrono::system_clock::duration t = std::chrono::system_clock::now() - T;
    if((500ms - t) > 0s)
        std::this_thread::sleep_for(500ms - t);
    T = std::chrono::system_clock::now();
    return 1;
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
std::vector<std::string> GetDate() {
    struct tm  tstruct;
    time_t     now = time(0);
    char       date[50];
    tstruct = *localtime(&now);

    strftime(date, sizeof(date), "%d-%m-%Y", &tstruct);
    std::string _day = GetDay(date);

    std::vector<std::string> status;
    status.push_back(_day);
    status.push_back(date);
    return status;
}

std::string GetTime() {
    struct tm  tstruct;
    time_t     now = time(0);
    tstruct = *localtime(&now);
    char       time[50];

    strftime(time, sizeof(time), "%X", &tstruct);
    return time;
}

int Time() {
    std::chrono::system_clock::time_point T = std::chrono::system_clock::now() + 1s;
    std::vector<std::string> S = GetDate();
    while(1) {
        if(!Run(T)) {
            return 1;
        }
        std::vector<std::string> Output;
        Output.push_back(R"($(printf ")" + TCol[0] + IDate + " " + TCol[1] + S.at(0) + " " + TCol[2] + S.at(1) + " " + TCol[3] + GetTime() + BDCol + R"("))");
        WriteFileLines(Output, TimeOutputFile);

#ifdef TimeMCOUT
        std::cout << Output.front() << std::endl;
        BreakPoint();
#endif
        return 0;
    }
}

int main() {
    while(1) {
        Time();
        std::this_thread::sleep_for(std::chrono::milliseconds(SleepTime));
    }
    return 0;
}
