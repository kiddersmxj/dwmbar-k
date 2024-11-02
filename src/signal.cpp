#include "../inc/signal.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <sstream>
#include <algorithm>
#include <iomanip> // For std::setfill and std::setw

// Helper function to format the speed and return the number and unit separately
std::pair<std::string, std::string> formatSpeed(double speed) {
    std::ostringstream formattedNumber;
    std::string unit;
    formattedNumber << std::setfill('0') << std::setw(3); // Ensure 3 digits with leading zeros

    if (speed >= 1e3) { // Convert to Gb/s
        formattedNumber << static_cast<int>(speed / 1e3);
        unit = "Gb/s";
    } else if (speed >= 1.0) { // Keep in Mb/s
        formattedNumber << static_cast<int>(speed);
        unit = "Mb/s";
    } else { // Convert to Kb/s
        formattedNumber << static_cast<int>(speed * 1e3);
        unit = "Kb/s";
    }
    return {formattedNumber.str(), unit};
}

void SignalModule::run() {
    std::string output = " ";

    std::string interface = getActiveInterface();
    if (interface.empty()) {
        std::cerr << "No active network interface found." << std::endl;
    }

    std::string rx_path = "/sys/class/net/" + interface + "/statistics/rx_bytes";
    std::string tx_path = "/sys/class/net/" + interface + "/statistics/tx_bytes";

    long prev_rx_bytes = readBytes(rx_path);
    long prev_tx_bytes = readBytes(tx_path);

    // Use the specified sleep time for interval calculation
    double sleep_interval_seconds = SignalSleepTime / 1000.0;

    while (true) {
        long current_rx_bytes = readBytes(rx_path);
        long current_tx_bytes = readBytes(tx_path);

        // Calculate speeds in Mb/s
        double rx_speed = (current_rx_bytes - prev_rx_bytes) * 8.0 / 1e6 / sleep_interval_seconds; // Convert bytes to Mb/s
        double tx_speed = (current_tx_bytes - prev_tx_bytes) * 8.0 / 1e6 / sleep_interval_seconds; // Adjust by sleep interval

        // Format the speeds to appropriate units
        auto [formatted_rx_number, rx_unit] = formatSpeed(rx_speed);
        auto [formatted_tx_number, tx_unit] = formatSpeed(tx_speed);

        // std::cout << "Download (rx): " << formatted_rx_number << rx_unit << std::endl;
        // std::cout << "Upload (tx): " << formatted_tx_number << tx_unit << std::endl;

        // Update previous values
        prev_rx_bytes = current_rx_bytes;
        prev_tx_bytes = current_tx_bytes;

        output = R"($(printf ")" + SCol[0] + IArrowDown + " " + SCol[1] \
                 + formatted_rx_number + " " + SCol[2] + rx_unit + SCol[3] + " " \
                 + IArrowUp + " " + SCol[4] + formatted_tx_number + " " + SCol[5] + tx_unit + BDCol + R"("))";

        updateOutput(output);

        std::this_thread::sleep_for(std::chrono::milliseconds(SignalSleepTime));  // Control loop frequency
    }
}

long SignalModule::readBytes(const std::string& path) {
    std::ifstream file(path);
    long bytes = 0;
    file >> bytes;
    return bytes;
}

std::string SignalModule::getActiveInterface() {
    std::ifstream file("/proc/net/dev");
    std::string line;
    std::vector<std::pair<std::string, long>> interfaces;

    // Skip the first two lines (header)
    std::getline(file, line);
    std::getline(file, line);

    // Parse the interfaces and their received bytes
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string interface;
        long rx_bytes;

        // Extract the interface name (before ':')
        std::getline(ss, interface, ':');
        interface.erase(std::remove_if(interface.begin(), interface.end(), ::isspace), interface.end());

        // Read rx_bytes (first field after interface name)
        ss >> rx_bytes;

        if (!interface.empty() && interface != "lo" && rx_bytes > 0) {
            interfaces.emplace_back(interface, rx_bytes);
        }
    }

    // Return the interface with the highest received bytes
    if (!interfaces.empty()) {
        return interfaces.front().first;
    } else {
        return ""; // Return empty if no interface found
    }
}
