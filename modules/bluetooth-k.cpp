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
    std::string Device = ExecCmd(R"(bluetoothctl info | grep Name | awk -F': ' '{print $2}')", 0, 0);
    Device = StripTrailingNL(Device);

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
        WriteFileLines(Output, BluetoothOutputFile);
        WriteFileLines(b, BluetoothDataFile);
    }
    Out = Device;
}

int main() {
    std::string Out = " ";
    std::vector<std::string> b(1);
    b.at(0) = "0";
    WriteFileLines(b, BluetoothDataFile);
    while(1) {
        Bluetooth(Out);
        std::this_thread::sleep_for(std::chrono::milliseconds(SleepTime));
        BreakPoint();
    }
    return 0;
}
