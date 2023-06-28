#include "../include/dwmbar-k.hpp"
#include <iostream>

int C = -1;

int Run() {
    if(C == PollClock(CDir)) {
        return 0;
    }
    C = PollClock(CDir);
    if((PollClock(CDir) % BluetoothFrq) == 0 || PollClock(CDir) == 0) {
        return 1;
    }
    return 0; 
}

void Bluetooth(std::string &Out) {
    if(!Run())
        return;
    std::vector<std::string> Output;
    std::string Device = k::ExecCmd(R"(bluetoothctl info | grep Name | awk -F': ' '{print $2}')", 0, 0);
    Device = k::StripTrailingNL(Device);

    std::vector<std::string> b(1);
    if(Device == "") {
        Device = " ";
        b.at(0) = "0";
    } else {
        Device = R"($(printf ")" + BlCol[0] + IBluetooth + " " + BlCol[1] + Device + R"("))";
        b.at(0) = "1";
    }

    if(Device != Out) {
        Output.push_back(Device);
        k::WriteFileLines(Output, BluetoothOutputFile);
        k::WriteFileLines(b, BluetoothDataFile);
    }
    Out = Device;
}

int main() {
    std::string Out = " ";
    std::vector<std::string> b(1);
    b.at(0) = "0";
    k::WriteFileLines(b, BluetoothDataFile);
    while(1) {
        Bluetooth(Out);
        std::this_thread::sleep_for(std::chrono::milliseconds(SleepTime));
        k::BreakPoint();
    }
    return 0;
}
