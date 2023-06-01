#include <ctime>
#include "../include/dwmbar-k.hpp" 

int C = -1;

int Run() {
	// If first run (or just been killed) run
	if(C == -1)
		return 1;
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
std::vector<std::string> GetDateAndTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       date[50];
    char       time[50];
    tstruct = *localtime(&now);

    strftime(date, sizeof(date), "%d-%m-%Y", &tstruct);
    std::string _day = GetDay(date);
    strftime(time, sizeof(time), "%X", &tstruct);

    std::vector<std::string> status;
    status.push_back(_day);
    status.push_back(date);
    status.push_back(time);
    return status;
}

int Time() {
    if(!Run()) {
        return 1;
    }
    std::vector<std::string> S = GetDateAndTime();
    std::vector<std::string> Output;
    Output.push_back(R"($(printf ")" + TCol[0] + IDate + " " + TCol[1] + S.at(0) + " " + TCol[2] + S.at(1) + " " + TCol[3] + S.at(2) + BDCol + R"("))");
    WriteFileLines(Output, TimeOutputFile);

#ifdef TimeMCOUT
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
