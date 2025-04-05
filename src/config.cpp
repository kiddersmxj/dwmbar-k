#include "../inc/config.hpp"

int initializeConfig(const std::string& config_file_path) {
    // Load configuration file using the singleton Config instance
    if (!k::config::Config::getInstance().load(config_file_path)) {
        std::cerr << "Failed to load config file: " << config_file_path << std::endl;
        // Handle error as needed
        return 1;
    }

    KCONFIG_VAR_REQUIRED(SleepTime, "sleep.sleeptime")
    KCONFIG_VAR(BluetoothSleepTime, "sleep.bluetooth", 500)
    KCONFIG_VAR(NetworkSleepTime, "sleep.network", 500)
    KCONFIG_VAR(MediaSleepTime, "sleep.media", 500)
    KCONFIG_VAR(VolumeSleepTime, "sleep.volume", 500)
    KCONFIG_VAR(WeatherSleepTime, "sleep.weather", 500)
    KCONFIG_VAR(BatterySleepTime, "sleep.battery", 500)
    KCONFIG_VAR(SignalSleepTime, "sleep.signal", 500)
    KCONFIG_VAR(CPUSleepTime, "sleep.cpu", 500)
    KCONFIG_VAR(ThermalThrottleSleepTime, "sleep.throttle", 500)
    KCONFIG_VAR(BrightnessSleepTime, "sleep.brightness", 500)
    KCONFIG_VAR(WinConnectSleepTime, "sleep.winconnect", 500)

    KCONFIG_ARRAY_REQUIRED(ModuleLayout, "layout.layout")

    std::cout << "Modules Layout: " << std::endl;
    for (const auto& module : ModuleLayout) {
        std::cout << "\t" << module << std::endl;
    }

    return 0;
}

