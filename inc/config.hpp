#ifndef Kconfig
#define Kconfig

#include <std-k.hpp>

#include <cstring>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>

using std::chrono_literals::operator""ms;

const std::string ConfigFile = "/home/kidders/.dwmbar.conf";

// Define configuration variables with default values using inline
inline std::vector<std::string> ModuleLayout = {};

inline int SleepTime; // How long while loop waits before re-execution in ms
inline int BluetoothSleepTime;
inline int NetworkSleepTime;
inline int MediaSleepTime;
inline int VolumeSleepTime;
inline int WeatherSleepTime;
inline int BatterySleepTime;
inline int SignalSleepTime;
inline int CPUSleepTime;
inline int ThermalThrottleSleepTime;
inline int BrightnessSleepTime;
inline int WinConnectSleepTime;

// Function to initialize global configuration variables
int initializeConfig(const std::string& filePath);

const int ModulesLength = 12;
const std::string Modules[ModulesLength] =  { 
    "network",
    "time",
    "weather",
    "bluetooth",
    "media",
    "battery",
    "volume",
    "signal",
    "cpu",
    "throttling",
    "brightness",
    "winconnect",
};

const std::string Separator = ";";

// const std::vector<std::string> ModuleLayout = {
//     "network",
//     "time",
//     Separator,
//     "cpu",
//     "throttling",
//     "brightness",
//     "bluetooth",
//     "signal",
//     "weather",
//     Separator,
//     "battery",
//     "volume",
//     "media",
// };

const std::string NoOutputCode = "NaN";

// Scale the volume based on individual system - 1 for off
const float VolScaler = 1.0;
const std::time_t t = std::time(0);
inline const std::tm* now = std::localtime(&t);
const std::string DT = getenv("logdatetime");
/* const std::string DT = std::to_string(now->tm_hour) + ':' + std::to_string(now->tm_min) \ */
/*                        + ':' + std::to_string(now->tm_sec) + '.' \ */
/*                        + std::to_string(now->tm_mday) + '-' \ */
/*                        + std::to_string(now->tm_mon + 1) + '-' \ */
/*                        + std::to_string(now->tm_year + 1900); */
const std::string TmpDir = "/tmp/dwmbar-k"; // Dir to store tmp files in
const std::string WDir = "/usr/local/bin/dwmbar-k"; // Working directory
const std::string TDir = TmpDir; // Full TmpDir path
const std::string Logfile = "/log/dwmbar-k/" + DT + ".dwmbar.log"; // Full DataDir path

const std::chrono::system_clock::duration Latency = 200ms; // How long while loop waits before re-execution in ms

// Volume levels as a percentage for corresponding icons
// Stored value is the bottom value for that icon
// Next value above is the top band for that icon
const int VHigh = 70; // 70-100
const int VMid = 40; // 40-69
const int VLow = 1; // 1-39
const int VMute = 0; // 0

// Battery levels as a percentage for corresponding icons
// Stored value is the bottom value for that icon
// Next value above is the top band for that icon
const int BFull = 90; // 90-100
const int BThreeQuart = 75; // 75-89
const int BHalf = 40; // 40 -74
const int BQuart = 20; // 20-39
const int BEmpty = 1; // Recomend non-zero value to catch broken battery

const std::string ModuleDelimeter = "|";
const std::string BarDelimeter = ";";

const std::string IInternet = R"(\xef\x82\xac)";

const std::string ICPU = R"(\xef\x8b\x9b)";

const std::string IArrowUp = R"(\xef\x85\xb6)";
const std::string IArrowDown = R"(\xef\x85\xb5)";

const std::string IDate = R"(\xef\x84\xb3)";

const std::string IVolMute = R"(\xef\x9a\xa9)";
const std::string IVolLow = R"(\xef\x80\xa6)";
const std::string IVolMid = R"(\xef\x80\xa7)";
const std::string IVolHigh = R"(\xef\x80\xa8)";

const std::string IBatteryFull = R"(\xef\x89\x80)";
const std::string IBatteryThreeQuart = R"(\xef\x89\x81)";
const std::string IBatteryHalf = R"(\xef\x89\x82)";
const std::string IBatteryQuart = R"(\xef\x89\x83)";
const std::string IBatteryEmpty = R"(\xef\x89\x84)";
const std::string ICharging = R"(\xef\x83\xa7)";

const std::string IPlay = R"(\xe2\x96\xb6)";
const std::string IPause = R"(\xe2\x8f\xb8)";

const std::string IBluetooth = R"(\xef\x8a\x94)";

const std::string IThermometer = R"(\xef\x8b\x87)";

const std::string ILight = R"(\xef\x83\xab)";

const std::string IHdd = R"(\xef\x82\xa0)";

// Colours
namespace Colour {
    const std::string Black = "^C0^";
    const std::string Red = "^C1^";
    const std::string Green = "^C2^";
    const std::string Yellow = "^C3^";
    const std::string Blue = "^C4^";
    const std::string Magenta = "^C5^";
    const std::string Cyan = "^C6^";
    const std::string White = "^C7^";
    const std::string Grey = "^C8^";
    const std::string AltRed = "^C9^";
    const std::string AltGreen = "^C10^";
    const std::string AltYellow = "^C11^";
    const std::string AltBlue = "^C12^";
    const std::string AltMagenta = "^C13^";
    const std::string AltCyan = "^C14^";
    const std::string AltWhite = "^C15^";
};

const std::string BDCol = Colour::Grey;

const int SignalNumColours = 6;
const std::string SCol[SignalNumColours] = {
    Colour::Magenta, Colour::Grey, Colour::Grey, Colour::Magenta, Colour::Grey, Colour::Grey
//              Icon                Number              Unit                Icon                Number              Unit
};
const int NetworkNumColours = 4;
const std::string NCol[NetworkNumColours] = {
    Colour::Magenta, Colour::Magenta, Colour::Grey, Colour::Magenta
//          Icon              AP Name         Private IP       Public IP
};
const int TimeNumColours = 4;
const std::string TCol[TimeNumColours] = {
    Colour::Green, Colour::Green, Colour::Grey, Colour::Green
//          Icon              Day               Date              Time
};
const int WeatherNumColours = 1;
const std::string WCol[WeatherNumColours] = {
    Colour::Yellow
//          Icon
};
const int VolumeNumColours = 2;
const std::string VCol[VolumeNumColours] = {
    Colour::Red, Colour::Grey
//          Icon            Percentage
};
const int MediaNumColours = 6;
const std::string MCol[MediaNumColours] = {
    Colour::Blue, Colour::Blue, Colour::Grey, Colour::Blue, Colour::Grey, Colour::Grey
//          Icon              Artist               Dash             Title           Brackets           Duration
};
const int BatteryNumColours = 7;
const std::string BCol[BatteryNumColours] = {
    Colour::Green, Colour::Green, Colour::Yellow, Colour::Yellow, Colour::Red, Colour::Grey, Colour::Grey
//        Icon(4/4)          Icon(3/4)          Icon(2/4)           Icon(1/4)          Icon(0/4)        Extra Icon     Percentage
};
const int CPUNumColours = 2;
const std::string CPUCol[CPUNumColours] = {
    Colour::Green, Colour::Grey
//        Icon                 Number
};
const int BrightnessNumColours = 2;
const std::string BrCol[BrightnessNumColours] = {
    Colour::Yellow, Colour::Grey
//        Icon                 Name
};
const int BluetoothNumColours = 2;
const std::string BlCol[BluetoothNumColours] = {
    Colour::Blue, Colour::Grey
//        Icon                 Name
};
const int ThrottlingNumColours = 1;
const std::string ThCol[ThrottlingNumColours] = {
    Colour::Red
//        Icon
};

const int WinConnectNumColours = 1;
const std::string WCCol[WinConnectNumColours] = {
    Colour::Blue
//        Icon
};

#endif 
