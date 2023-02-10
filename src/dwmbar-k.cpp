#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <cstring>
#include "../include/dwmbar-k.hpp"
namespace fs = std::filesystem;

void RunModule(std::string Module) {
    std::cout << Module << std::endl;
    std::stringstream LaunchCmd;
    LaunchCmd << "$HOME/devel/dwmbar-k/./launch.sh "<< Module << " &";
    system(LaunchCmd.str().c_str());
}

std::vector<std::string> GetModuleOutput(std::string Module) {
    return ReadFileLines(HOME + "/devel/dwmbar-k/.tmp/" + Module + ".txt");
}

std::string ParseModuleNo(std::string ModuleNo) {
    std::cout << ModuleNo << " ";
    // -1 to account for 0 based indexing
    return Modules[stoi(ModuleNo) - 1];
}

void InitClock() {
#ifdef COUT
    std::cout << "Clock=" << PollClock(CDir);
#endif
    if(PollClock(CDir) != 0) {
        for (const auto & entry : fs::directory_iterator(CDir)) {
	        rename(entry.path(), CDir + "/0");
        }
    }
#ifdef COUT
    std::cout << "...Clock=" << PollClock(CDir) << std::endl;
#endif
}

int main() {
    InitClock();
    std::cout << PollClock(CDir) << std::endl;
    std::string result = ExecCmd(R"(mkdir $HOME/devel/dwmbar-k/.tmp > /dev/null 2>&1 || echo 1)", 0, 0);
    std::cout << "CMD-" << result << "-" << std::endl;
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
                std::cout << ";" << std::endl;
            } else {
				std::vector<std::string> Output = GetModuleOutput(ParseModuleNo(substr));
				VPrint(Output);
				std::cout << std::endl;
            }
		}

		BreakPoint();
#ifndef NORUN
    }
#endif

    return 1;
}

