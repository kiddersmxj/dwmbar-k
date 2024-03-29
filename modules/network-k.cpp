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
    k::ExecCmd(R"(nmcli -a | grep 'Wired connection' | awk 'NR==1{print $1}')", Name);
    if(Name == "")
        k::ExecCmd(R"(nmcli -t -f active,ssid dev wifi | grep '^yes' | cut -c 5-)", Name);
    Name = k::StripTrailingNL(Name);
    return Name;
}

std::string GetPublicIP() {
    std::string IP;
    k::ExecCmd(R"(curl -s https://ipinfo.io/ip)", IP);
    IP = k::StripTrailingNL(IP);
    return IP;
}

std::string GetPrivateIP() {
    std::string IP;
    k::ExecCmd(R"(nmcli -a | grep -m 1 'inet4 ' | awk '{print $2}')", IP);
    IP = k::StripTrailingNL(IP);
    return IP;
}

int Network(std::string &Out) {
    if(!Run())
        return 1;
    std::vector<std::string> Output;
    if(GetConnectionName() != "")
        Output.push_back(R"($(printf ")" + NCol[0] + IInternet + " " + NCol[1] + GetConnectionName() + " " + NCol[2] + GetPrivateIP() + " " + NCol[3] + GetPublicIP() + BDCol + R"("))");
    else
        Output.push_back(" ");
    if(Out != Output.at(0))
        k::WriteFileLines(Output, NetworkOutputFile);
    Out = Output.at(0);
    return 0;
}

int main() {
    std::string Out = " ";
    while(1) {
        Network(Out);
        std::this_thread::sleep_for(std::chrono::milliseconds(SleepTime));
    }
    return 0;
}

