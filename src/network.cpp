#include "../inc/network.hpp"
#include <vector>
#include <chrono>
#include <thread>

void NetworkModule::run() {
    std::string output = " ";
    while (true) {
        std::string connName = getConnectionName();
        std::string privateIP = getPrivateIP();
        std::string publicIP = getPublicIP();

        if (!connName.empty()) {
            output = R"($(printf ")" + NCol[0] + IInternet + " " + NCol[1] + connName + " " \
                     + NCol[2] + privateIP + NCol[3] + " " + publicIP + BDCol + R"("))";
            // std::cout << "NetworkModule: Updating output to " << output << std::endl;  // Debug

            updateOutput(output);
        } else {
            // std::cout << "NetworkModule: No Network" << std::endl;  // Debug
            updateOutput(NoOutputCode);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(NetworkSleepTime));  // Control loop frequency
    }
}

std::string NetworkModule::getConnectionName() {
    std::string name;
    k::ExecCmd("nmcli -a | grep 'Wired connection' | awk 'NR==1{print $1}'", name);
    if (name.empty()) {
        k::ExecCmd("nmcli -t -f active,ssid dev wifi | grep '^yes' | cut -c 5-", name);
    }
    return k::StripTrailingNL(name);
}

std::string NetworkModule::getPublicIP() {
    std::string ip;
    k::ExecCmd("curl -s https://ipinfo.io/ip", ip);
    return k::StripTrailingNL(ip);
}

std::string NetworkModule::getPrivateIP() {
    std::string ip;
    k::ExecCmd("nmcli -a | grep -m 1 'inet4 ' | awk '{print $2}'", ip);
    return k::StripTrailingNL(ip);
}

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

