#include "../include/dwmbar-k.hpp" 

int C = -1;

int Run() {
    int Clk = PollClock(CDir);
    if(C == Clk) {
        return 0;
    }
    C = Clk;
    if((Clk % NetworkFrq) == 0 || Clk == 0)
        return 1;
    return 0; 
}

std::string GetConnectionName() {
    std::string Name;
    Name = ExecCmd(R"(nmcli -a | grep 'Wired connection' | awk 'NR==1{print $1}')", 0, 0);
    if(Name == "")
        Name = ExecCmd(R"(nmcli -t -f active,ssid dev wifi | grep '^yes' | cut -c 5-)", 0, 0);
    Name = StripTrailingNL(Name);
    return Name;
}

std::string GetPublicIP() {
    std::string IP;
    IP = ExecCmd(R"(curl -s https://ipinfo.io/ip)", 0, 0);
    IP = StripTrailingNL(IP);
    return IP;
}

std::string GetPrivateIP() {
    std::string IP;
    IP = ExecCmd(R"(nmcli -a | grep -m 1 'inet4 ' | awk '{print $2}')", 0, 0);
    IP = StripTrailingNL(IP);
    return IP;
}

int Network() {
    if(!Run())
        return 1;
    std::vector<std::string> Output;
    if(GetConnectionName() != "")
        Output.push_back(R"($(printf ")" + NCol[0] + IInternet + " " + NCol[1] + GetConnectionName() + " " + NCol[2] + GetPrivateIP() + " " + NCol[3] + GetPublicIP() + BDCol + R"("))");
    else
        Output.push_back(" ");
    WriteFileLines(Output, NetworkOutputFile);
    return 0;
}

int main() {
    while(1) {
        Network();
        std::this_thread::sleep_for(std::chrono::milliseconds(SleepTime));
    }
    return 0;
}

