#include <sstream>
#include <numeric> 
#include <fstream>
#include <filesystem>
#include "../include/dwmbar-k.hpp"
namespace fs = std::filesystem;

int C = 0;

void InitClock();
void InitDirs();
void PulseClock();
int Enabled(int i);
void RunModules();
void KillModules();
void RunModule(std::string Module);
void KillModule(std::string Module);
std::string GetModuleOutput(std::string Module);
std::string ParseModuleNo(std::string ModuleNo);
void XSR(std::string Body);
std::string ParseXSR(std::vector<std::string> VectorOutput);

int main() {
    KillModules();
    InitDirs();
    InitClock();
    
    std::vector<std::string> Output;

    // Only runs if .bashrc set $dwmbar to 1
    while(getenv("dwmbar")) {
        RunModules();
        if(C % ClockFrq == 0) {
	    	for(std::string substr: ModuleLayout) {
                if(substr == ";") {
					Output.push_back(";");
#ifdef COUT
                    std::cout << ";" << std::endl;
#endif
                } else {
					std::string Out = GetModuleOutput(ParseModuleNo(substr));
					// Make sure not parsing empty module data
					if(Out != "")
						Output.push_back(Out);
                }
		    }
#ifdef OCOUT
			std::cout << "-" << std::endl;
			VPrint(Output);
			std::cout << "-" << std::endl;
#endif
			
			XSR(ParseXSR(Output));

		    BreakPoint();
            PulseClock();
            Output.clear();
        }
        C++;
    }
    return 1;
}

void RunModule(std::string Module) {
#ifdef COUT
    std::cout << Module << std::endl;
#endif
    std::stringstream LaunchCmd;
    LaunchCmd << "$HOME/devel/dwmbar-k/./launch.sh "<< Module << " &";
    system(LaunchCmd.str().c_str());
}

void KillModule(std::string Module) {
#ifdef COUT
    std::cout << Module << std::endl;
#endif
    std::stringstream KillCmd;
    KillCmd << "pkill "<< Module << " &";
    system(KillCmd.str().c_str());
}

std::string GetModuleOutput(std::string Module) {
	if(Module == "Disabled")
		return "";
    std::vector<std::string> Output = ReadFileLines(ODir + "/" + Module + ".txt");
	std::string O;
	for(std::string Out: Output) {
			O = Out;
	}
#ifdef COUT
	std::cout << "Output=" << O << std::endl;
    return O;
#endif
}

std::string ParseModuleNo(std::string ModuleNo) {
#ifdef COUT
    std::cout << ModuleNo << " ";
#endif
    // -1 to account for 0 based indexing
	int ModNo = stoi(ModuleNo) - 1;
	if(EnabledModules[ModNo])
		return Modules[ModNo];
	else
		return "Disabled";
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

void InitDirs() {
	ExecCmd(R"(mkdir )" + ODir + R"( > /dev/null 2>&1 || echo 1)", 0, 0);
    ExecCmd(R"(mkdir )" + TDir + R"( > /dev/null 2>&1 || echo 1)", 0, 0);
    ExecCmd(R"(mkdir )" + CDir + R"( > /dev/null 2>&1 || echo 1)", 0, 0);
    ExecCmd(R"(mkdir )" + ODir + R"( > /dev/null 2>&1 || echo 1)", 0, 0);
    ExecCmd(R"(mkdir )" + DDir + R"( > /dev/null 2>&1 || echo 1)", 0, 0);
}

int Enabled(int i) {
    return EnabledModules[i];
}

void RunModules() {
#ifndef NORUN
        for(int i=0; i<ModulesLength; i++) {
            if(ExecCmd("ps -a | grep " + Modules[i], 0, 0) == "") {
                if(Enabled(i))
                    RunModule(Modules[i]);
            }
        }
#endif
}

void XSR(std::string Body) {
	std::string Cmd = R"(xsetroot -name "$(printf ")" + Body + R"(") ")";
	std::cout << Cmd << std::endl;
    ExecCmd(Cmd, 0, 0);
}

std::string ParseXSR(std::vector<std::string> VectorOutput) {
	std::string XSRBody = "";
	for( std::string ModOutput: VectorOutput)
		XSRBody += R"($(printf ")" + ModOutput + R"("))";
	std::cout << XSRBody << std::endl;
	return XSRBody;
}

void KillModules() {
        for(int i=0; i<ModulesLength; i++) {
            if(ExecCmd("ps -a | grep " + Modules[i], 0, 0) != "") {
                KillModule(Modules[i]);
            }
        }
}
