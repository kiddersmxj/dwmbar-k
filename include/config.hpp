#ifndef Kconfig
#define Kconfig

#include <std-k.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>

using std::chrono_literals::operator""ms;

const int ModulesLength = 7;
const std::string Modules[ModulesLength] =  { "network-k", "time-k", "weather-k", "bluetooth-k", "media-k", "battery-k", "volume-k" };
const int EnabledModules[ModulesLength] =   { 1,           1,        1,           1,             1,         1,           1 };
const int ParentControlled[ModulesLength] = { 0,           0,        0,           0,             0,         0,           1 };

const std::vector<std::string> ModuleLayout = { "1", "4", "2", ";", "6", "3", "7", "5" };

// Scale the volume based on individual system - 1 for off
const float VolScaler = 1.0;
const std::time_t t = std::time(0);
inline const std::tm* now = std::localtime(&t);
const std::string DT = std::to_string(now->tm_hour) + ':' + std::to_string(now->tm_min) \
                       + ':' + std::to_string(now->tm_sec) + '.' \
                       + std::to_string(now->tm_mday) + '-' \
                       + std::to_string(now->tm_mon + 1) + '-' \
                       + std::to_string(now->tm_year + 1900);
const std::string TmpDir = "/tmp/dwmbar-k"; // Dir to store tmp files in
const std::string WDir = "/usr/local/bin/dwmbar-k"; // Working directory
const std::string TDir = TmpDir; // Full TmpDir path
const std::string OutputDir = "output"; // Name of output files dir
const std::string ODir = TDir + "/" + OutputDir; // Full OutputDir path
const std::string ClockDir = "clock"; // Name of clock file dir
const std::string CDir = TDir + "/" + ClockDir; // Full ClockDir path
const std::string DataDir = "data"; // Name of data file dir
const std::string DDir = TDir + "/" + DataDir; // Full DataDir path
const std::string Logfile = "/log/dwmbar-k/" + DT + ".dwmbar"; // Full DataDir path
// Output file store locations
const std::string NetworkOutputFile = ODir + "/network-k.txt";
const std::string TimeOutputFile = ODir + "/time-k.txt";
const std::string WeatherOutputFile = ODir + "/weather-k.txt";
const std::string VolumeOutputFile = ODir + "/volume-k.txt";
const std::string MediaOutputFile = ODir + "/media-k.txt";
const std::string BatteryOutputFile = ODir + "/battery-k.txt";
const std::string BluetoothOutputFile = ODir + "/bluetooth-k.txt";
// Data file store locations
const std::string NetworkDataFile = DDir + "/network.txt";
const std::string TimeDataFile = DDir + "/time.txt";
const std::string WeatherDataFile = DDir + "/weather.txt";
const std::string VolumeDataFile = DDir + "/volume.txt";
const std::string MediaDataFile = DDir + "/media.txt";
const std::string BatteryDataFile = DDir + "/battery.txt";
const std::string BluetoothDataFile = DDir + "/bluetooth.txt";

const int MaxClock = 100; // Maximum clock value before reset
const int ClockFrq = 1; // How many in prog clock cycles before pulse
// How many clock cycles each module runs
const int NetworkFrq = 50;
const int TimeFrq = 1;
const int WeatherFrq = 100;
const int VolumeFrq = 5;
const int MediaFrq = 5;
const int BatteryFrq = 3;
const int BluetoothFrq = 5;

const std::chrono::system_clock::duration Latency = 200ms; // How long while loop waits before re-execution in ms
const int SleepTime = 50; // How long while loop waits before re-execution in ms

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

const int BluetoothNumColours = 7;
const std::string BlCol[BluetoothNumColours] = {
    Colour::Blue, Colour::Grey
//        Icon                 Name
};

#endif 
