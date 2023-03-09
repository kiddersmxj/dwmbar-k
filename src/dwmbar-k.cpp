#include <cstdint>
#include <sstream>
#include <numeric> 
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include "../include/dwmbar-k.hpp"
namespace fs = std::filesystem;

class InternalClock {
	public:
		int Get();
		void Pulse();
	private:
		void Reset();
		int C = 1;
};

class Clock {
    public:
        void Init();
        int Get();
        void Pulse();
    private:
        void Reset();
        int C = 1;
};

void InitDirs();
int Enabled(int i);
void RunModules();
void KillModules();
void RunModule(std::string Module);
void KillModule(std::string Module);
std::string GetModuleOutput(std::string Module);
std::string ParseModuleNo(std::string ModuleNo);
void XSR(std::string Body);
std::string ParseXSR(std::vector<std::string> VectorOutput);
int GetIndex(std::vector<std::string> v, std::string K);

int main() {
    KillModules();
    InitDirs();

    class Clock Clock;
    Clock.Init();

	class InternalClock Internal;
    
    // Plus one to allow for BarDelimeter
    std::string Output[ModulesLength + 1];
    std::string Backup[ModulesLength + 1];

    while(1) {
        RunModules();
        if(Internal.Get() % ClockFrq == 0) {
            int i = 0;
	    	for(std::string substr: ModuleLayout) {
                if(substr == BarDelimeter) {
                    Output[i] = BarDelimeter;
                    Backup[i] = BarDelimeter;
#ifdef COUT
                    std::cout << BarDelimeter << std::endl;
#endif
                } else {
                    std::string Out = GetModuleOutput(ParseModuleNo(substr));
                    if(Out != "") {
                        Output[i] = Out;
                        Backup[i] = Out;
                    } else {
                        Output[i] = Backup[i];
                        // TODO: Make a warning if Output is missed too many times
                    }
                }
                i++;
		    }
#ifdef OCOUT
			std::cout << "-" << std::endl;
            for(int i = 0; i<(ModulesLength + 1); i++)
                std::cout << Output[i] << std::endl;
			std::cout << "-" << std::endl;
#endif
			
            std::vector<std::string> OutputVector(Output, Output + (ModulesLength + 1));
			XSR(ParseXSR(OutputVector));

		    BreakPoint();
            Clock.Pulse();
        }
        Internal.Pulse();
    }
    return 1;
}

int InternalClock::Get() {
	return C;
}

void InternalClock::Pulse() {
    if(C == MaxClock)
        Reset();
    else
	    C++;
}

void InternalClock::Reset() {
    C = 1;
}

void Clock::Init() {
#ifdef COUT
    std::cout << "Clock=" << PollClock(CDir);
#endif
    if(PollClock(CDir) == -1) {
        Touch(CDir + "/1");
    } else if(PollClock(CDir) != 1)
        Reset();
#ifdef COUT
    std::cout << "...Clock=" << PollClock(CDir) << std::endl;
#endif
}

void Clock::Pulse() {
#ifdef COUT
    std::cout << "Clock=" << PollClock(CDir);
#endif
    int Pulse = PollClock(CDir);
    if(Pulse == MaxClock)
        Reset();
    else {
        Pulse++;
        for (const auto & entry : fs::directory_iterator(CDir))
	        rename(entry.path(), CDir + "/" + std::to_string(Pulse));
    }
#ifdef COUT
    std::cout << "...Clock=" << PollClock(CDir) << std::endl;
#endif
} 

void Clock::Reset() {
        for (const auto & entry : fs::directory_iterator(CDir))
	        rename(entry.path(), CDir + "/1");
}

void RunModule(std::string Module) {
#ifdef COUT
    std::cout << Module << std::endl;
#endif
    std::stringstream LaunchCmd;
    LaunchCmd << "$HOME/devel/dwmbar-k/bin/modules/" << Module << " &";
    std::cout << LaunchCmd.str().c_str() << std::endl;
    system(LaunchCmd.str().c_str());
}

void KillModule(std::string Module) {
#ifdef COUT
    std::cout << Module << std::endl;
#endif
    std::stringstream KillCmd;
    KillCmd << "pkill "<< Module << " 2> /dev/null &";
    ExecCmd(KillCmd.str().c_str(), 0, 0);
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
#endif
    return O;
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
	std::string Cmd = R"(xsetroot -name ")" + Body + R"(")";
#ifdef COUT
	std::cout << Cmd << std::endl;
#endif
    ExecCmd(Cmd, 0, 0);
}

std::string ParseXSR(std::vector<std::string> OutputVector) {
#ifdef COUT
    std::cout << std::endl;
#endif
	std::string XSRBody = "";
    // Make sure previous Output was not empty or BarDelimeter
    int Escape = 0;
    for(std::string Output: OutputVector) {
#ifdef COUT
        std::cout << "-" << Output << "-" << std::endl;
#endif
        if(Output != BarDelimeter && Output != "" && Escape == 0)
            XSRBody += " " + ModuleDelimeter;
        else if(Escape && Output != BarDelimeter && Output != "")
            Escape = 0;
        else
            Escape = 1;
        if(Output != "")
		    XSRBody += " " + Output;
    }
#ifdef COUT
	std::cout << XSRBody << std::endl;
#endif
	return XSRBody;
}

void KillModules() {
        for(int i=0; i<ModulesLength; i++) {
            if(ExecCmd("ps -a | grep " + Modules[i], 0, 0) != "") {
                KillModule(Modules[i]);
            }
        }
}

