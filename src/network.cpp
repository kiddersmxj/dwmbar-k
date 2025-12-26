#include "../inc/network.hpp"
#include <vector>
#include <chrono>
#include <thread>
#include <iostream>

namespace {
    // Determine the active interface and its IPv4 address.
    // Prefers the interface used for internet routing (ip route get 1.1.1.1),
    // but ignores interfaces whose operstate is not "up".
    // Falls back to the first "scope global" IPv4 address on an "up" interface.
    void resolve_active_interface(std::string &iface, std::string &ip) {
        std::string out;

        k::ExecCmd(
            "sh -c '"
            "get_route() {\n"
            "  if ! ip -4 route get 1.1.1.1 >/dev/null 2>&1; then\n"
            "    return 1;\n"
            "  fi\n"
            "  set -- $(ip -4 route get 1.1.1.1 2>/dev/null);\n"
            "  dev=\"\"; src=\"\";\n"
            "  while [ \"$#\" -gt 0 ]; do\n"
            "    case \"$1\" in\n"
            "      dev) dev=\"$2\"; shift 2; continue ;;\n"
            "      src) src=\"$2\"; shift 2; continue ;;\n"
            "    esac\n"
            "    shift\n"
            "  done\n"
            "  if [ -n \"$dev\" ] && [ -n \"$src\" ] &&\n"
            "     [ \"$(cat /sys/class/net/$dev/operstate 2>/dev/null)\" = \"up\" ]; then\n"
            "    printf \"%s %s\\n\" \"$dev\" \"$src\";\n"
            "    return 0;\n"
            "  fi\n"
            "  return 1;\n"
            "}\n"
            "\n"
            "# Prefer route-based selection, but fall back if it yields nothing usable\n"
            "if ! get_route; then\n"
            "  ip -4 -o addr show scope global | while read -r _ dev addr _; do\n"
            "    if [ \"$(cat /sys/class/net/$dev/operstate 2>/dev/null)\" != \"up\" ]; then\n"
            "      continue\n"
            "    fi\n"
            "    ip=${addr%/*};\n"
            "    if [ -n \"$dev\" ] && [ -n \"$ip\" ]; then\n"
            "      printf \"%s %s\\n\" \"$dev\" \"$ip\";\n"
            "      break;\n"
            "    fi\n"
            "  done\n"
            "fi'\n",
            out
        );

        out = k::StripTrailingNL(out);
        iface.clear();
        ip.clear();

        auto pos = out.find(' ');
        if (pos == std::string::npos) {
            return;
        }

        iface = out.substr(0, pos);
        ip    = out.substr(pos + 1);
    }
}

void NetworkModule::run() {
    std::string output = " ";

    while (true) {
        std::string iface;
        std::string privateIP;
        resolve_active_interface(iface, privateIP);

        if (!iface.empty()) {
            std::string publicIP = getPublicIP();

            // iface will be turned into SSID in getConnectionName() for Wi-Fi,
            // but here we want the raw interface name in case you prefer it.
            std::string displayName = getConnectionName();

            output = NCol[0] + IInternet + " " + NCol[1] + displayName + " " +
                     NCol[2] + privateIP + NCol[3] + " " + publicIP + BDCol;

            updateOutput(output);
        } else {
            updateOutput(NoOutputCode);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(NetworkSleepTime));
    }
}

std::string NetworkModule::getConnectionName() {
    std::string iface;
    std::string ip;
    resolve_active_interface(iface, ip);

    if (iface.empty()) {
        return "";
    }

    // Determine interface type (ethernet/wifi/…)
    std::string type;
    {
        std::string cmd =
            "nmcli -t -f DEVICE,TYPE dev status | "
            "grep '^" + iface + ":' | head -n1 | cut -d: -f2";
        k::ExecCmd(cmd.c_str(), type);
        type = k::StripTrailingNL(type);
    }

    // If this is Wi-Fi, return the SSID instead of the device name
    if (type == "wifi") {
        std::string ssid;
        k::ExecCmd(
            "nmcli -t -f ACTIVE,SSID dev wifi | "
            "awk -F: '$1==\"yes\" {print $2; exit}'",
            ssid
        );
        ssid = k::StripTrailingNL(ssid);
        if (!ssid.empty()) {
            return ssid;
        }
    }

    // Fallback: just return the interface name
    return iface;
}

std::string NetworkModule::getPrivateIP() {
    std::string iface;
    std::string ip;
    resolve_active_interface(iface, ip);
    return ip;
}

std::string NetworkModule::getPublicIP() {
    std::string ip;
    k::ExecCmd("curl -s https://ipinfo.io/ip", ip);
    return k::StripTrailingNL(ip);
}

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

