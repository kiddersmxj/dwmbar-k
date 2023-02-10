#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <cstring>
#include "../include/dwmbar-k.hpp"
namespace fs = std::filesystem;

int C = 0;

void RunModule(std::string Module) {
#ifdef COUT
    std::cout << Module << std::endl;
#endif
    std::stringstream LaunchCmd;
    LaunchCmd << "$HOME/devel/dwmbar-k/./launch.sh "<< Module << " &";
    system(LaunchCmd.str().c_str());
}

std::vector<std::string> GetModuleOutput(std::string Module) {
    return ReadFileLines(HOME + "/devel/dwmbar-k/.tmp/" + Module + ".txt");
}

std::string ParseModuleNo(std::string ModuleNo) {
#ifdef COUT
    std::cout << ModuleNo << " ";
#endif
    // -1 to account for 0 based indexing
    return Modules[stoi(ModuleNo) - 1];
}

void InitClock() {
#ifdef COUT
    std::cout << "Clock=" << PollClock(CDir);
#endif
    if(PollClock(CDir) == -1) {
        Touch(CDir + "/0");
    } else if(PollClock(CDir) != 0) {
        for (const auto & entry : fs::directory_iterator(CDir))
	        rename(entry.path(), CDir + "/0");
    }
#ifdef COUT
    std::cout << "...Clock=" << PollClock(CDir) << std::endl;
#endif
}

void PulseClock() {
#ifdef COUT
    std::cout << "Clock=" << PollClock(CDir);
#endif
    int Pulse = PollClock(CDir);
    Pulse++;
    for (const auto & entry : fs::directory_iterator(CDir))
	    rename(entry.path(), CDir + "/" + std::to_string(Pulse));
#ifdef COUT
    std::cout << "...Clock=" << PollClock(CDir) << std::endl;
#endif
} 

int main() {
    InitClock();
    std::string result = ExecCmd(R"(mkdir $HOME/devel/dwmbar-k/.tmp > /dev/null 2>&1 || echo 1)", 0, 0);
#ifdef COUT
    std::cout << "CMD-" << result << "-" << std::endl;
#endif
    if(result == "") {
        std::cout << "mkdir error";
        return 1;
    }

#ifndef NORUN
    // Only runs if .bashrc set $dwmbar to 1
    while(getenv("dwmbar")) {
        for(int i=0; i<ModulesLength; i++) {
            if(ExecCmd("ps -a | grep " + Modules[i], 0, 0) == "") {
                RunModule(Modules[i]);
            }
        }
#endif

		for(std::string substr: ModuleLayout) {
            if(substr == ";") {
#ifdef COUT
                std::cout << ";" << std::endl;
#endif
            } else {
				std::vector<std::string> Output = GetModuleOutput(ParseModuleNo(substr));
#ifdef COUT
				VPrint(Output);
				std::cout << std::endl;
#endif
            }
		}

		BreakPoint();
        PulseClock();
#ifndef NORUN
    }
#endif

    return 1;
}

