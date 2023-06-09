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
void RefreshDirs();
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
    RefreshDirs();

    class Clock Clock;
    Clock.Init();

	class InternalClock Internal;
    
    // Plus one to allow for BarDelimeter
    std::string Output[ModulesLength + 1];
    std::string Backup[ModulesLength + 1];

    while(1) {
        std::chrono::system_clock::time_point T = std::chrono::system_clock::now();
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
            std::chrono::system_clock::duration t = std::chrono::system_clock::now() - T;
            if((200ms - t) > 0s)
                std::this_thread::sleep_for(200ms - t);
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
        Touch(CDir + "/0");
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

void RefreshDirs() {
	ExecCmd(R"(rm -rf )" + ODir + R"( > /dev/null 2>&1 || echo 1)", 0, 0);
    ExecCmd(R"(rm -rf )" + CDir + R"( > /dev/null 2>&1 || echo 1)", 0, 0);
    ExecCmd(R"(rm -rf )" + DDir + R"( > /dev/null 2>&1 || echo 1)", 0, 0);
    InitDirs();
}

void InitDirs() {
	ExecCmd(R"(mkdir )" + ODir + R"( > /dev/null 2>&1 || echo 1)", 0, 0);
    ExecCmd(R"(mkdir )" + TDir + R"( > /dev/null 2>&1 || echo 1)", 0, 0);
    ExecCmd(R"(mkdir )" + CDir + R"( > /dev/null 2>&1 || echo 1)", 0, 0);
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
    std::vector<int> Empty;
    bool Escape = 1;
    int i = 0;

    // Make sure previous Output was not empty or BarDelimeter
    for(std::string Output: OutputVector) {
        if(Output != " " && Output != "" && Output != BarDelimeter)
#ifdef COUT
        std::cout << "-" << Output << "-" << std::endl;
#endif
            Empty.push_back(1);
        else
            Empty.push_back(0);
#ifdef COUT
    std::cout << "E (" << i << "): " << Escape << std::endl;
	std::cout << XSRBody << std::endl << std::endl;
#endif
    }

    for(int I: Empty) {
        if(I == 1) {
            if(Escape == 0)
                XSRBody += " " + ModuleDelimeter;
            else
                Escape = 0;
        } else if(OutputVector.at(i) == BarDelimeter)
            Escape = 1;
        if(OutputVector.at(i) != " " && OutputVector.at(i) != "")
		    XSRBody += " " + OutputVector.at(i);
        i++;
    }

	return XSRBody;
}

void KillModules() {
        for(int i=0; i<ModulesLength; i++) {
            if(ExecCmd("ps -a | grep " + Modules[i], 0, 0) != "") {
                KillModule(Modules[i]);
            }
        }
}

