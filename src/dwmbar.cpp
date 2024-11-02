#include "../inc/dwmbar.hpp"
#include <thread>
#include <iostream>
#include <vector>

int main() {

    NetworkModule networkModule;
    BluetoothModule bluetoothModule;
    MediaModule mediaModule;
    TimeModule timeModule;
    VolumeModule volumeModule;
    WeatherModule weatherModule;
    BatteryModule batteryModule;
    SignalModule signalModule;
    CPUModule cpuModule;

    std::thread networkThread(&NetworkModule::run, &networkModule);
    std::thread bluetoothThread(&BluetoothModule::run, &bluetoothModule);
    std::thread mediaThread(&MediaModule::run, &mediaModule);
    std::thread timeThread(&TimeModule::run, &timeModule);
    std::thread volumeThread(&VolumeModule::run, &volumeModule);
    std::thread weatherThread(&WeatherModule::run, &weatherModule);
    std::thread batteryThread(&BatteryModule::run, &batteryModule);
    std::thread signalThread(&SignalModule::run, &signalModule);
    std::thread cpuThread(&CPUModule::run, &cpuModule);

    std::vector<std::pair<std::string, Module*>> modules = {
        {"network", &networkModule},
        {"bluetooth", &bluetoothModule},
        {"media", &mediaModule},
        {"time", &timeModule},
        {"volume", &volumeModule},
        {"weather", &weatherModule},
        {"battery", &batteryModule},
        {"signal", &signalModule},
        {"cpu", &cpuModule},
    };

    while (true) {
        // Collect output from modules
        std::vector<std::pair<std::string, std::string>> outputVector;

        for (auto& [name, mod] : modules) {
            std::string output;
            std::unique_lock<std::mutex> lock(mod->getOutputMutex());
            if (mod->getOutputCV().wait_for(lock, std::chrono::milliseconds(1), [&]{ return !mod->getOutputNoLock().empty(); })) {
                output = mod->getOutputNoLock();
                if(output == "NaN") output = "";
                outputVector.emplace_back(name, output);  // Add name-output pair to vector
            }
        }

        // Print all collected outputs
        // for (const auto& [name, output] : outputVector) {
        //     std::cout << "Module: " << name << " | Output: " << output << std::endl;
        // }

        // Order the outputs based on ModuleLayout
        std::vector<std::string> OrderedVector;
        for (const auto& moduleName : ModuleLayout) {
            if (moduleName == ";") {
                // Add the separator directly to OrderedVector
                OrderedVector.push_back(";");
            } else {
                // Find the corresponding output in outputVector
                auto it = std::find_if(outputVector.begin(), outputVector.end(),
                                       [&](const std::pair<std::string, std::string>& p) { return p.first == moduleName; });
                if (it != outputVector.end()) {
                    OrderedVector.push_back(it->second);  // Add the output to OrderedVector
                }
            }
        }

        // Print the ordered outputs
        // std::cout << "Ordered Output: ";
        // for (const auto& output : OrderedVector) {
        //     std::cout << output << " ";
        // }
        // std::cout << std::endl;

        XSR(ParseXSR(OrderedVector));

        std::this_thread::sleep_for(std::chrono::milliseconds(SleepTime));
    }

    networkThread.join();
    bluetoothThread.join();
    mediaThread.join();
    timeThread.join();
    volumeThread.join();
    weatherThread.join();
    batteryThread.join();
    signalThread.join();
    cpuThread.join();

    return 0;
}

int Enabled(int i) {
    return EnabledModules[i];
}

void XSR(std::string Body) {
	std::string Cmd = R"(xsetroot -name ")" + Body + R"(")";
    k::ExecCmd(Cmd);
}

std::string ParseXSR(std::vector<std::string> OutputVector) {
	std::string XSRBody = "";
    std::vector<int> Empty;
    bool Escape = 1;
    int i = 0;

    // Make sure previous Output was not empty or BarDelimeter
    for(std::string Output: OutputVector) {
        if(Output != " " && Output != "" && Output != BarDelimeter)
            Empty.push_back(1);
        else
            Empty.push_back(0);
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

std::string ParseModuleNo(std::string ModuleNo) {
    // -1 to account for 0 based indexing
	int ModNo = stoi(ModuleNo) - 1;
	if(EnabledModules[ModNo])
		return Modules[ModNo];
	else
		return "Disabled";
}

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

