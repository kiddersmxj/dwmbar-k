#ifndef Kstd
#define Kstd

#include <iostream>
#include <vector>
#include <cstring>

void BreakPoint();
std::string ExecCmd(const std::string cmd, const int Output, int ExitStatus);
void WriteFileLines(std::vector<std::string> Lines, std::string File);
std::vector<std::string> ReadFileLines(std::string File);
void VPrint(std::vector<std::string> Input);
std::vector<std::string> SplitStr(std::string String, const char* Delim);
int PollClock(const std::string Dir);
void Touch(std::string File);
void Data(std::string Text, std::string File);
std::string StripTrailingNL(std::string Input);

#endif
