#ifndef Kconfig
#define Kconfig
// As much as it pains me to include t makes YCM happy
#ifndef RUN
#include <string> 
#include <vector> 
#endif

const int ModulesLength = 6;
const std::string Modules[ModulesLength] = { "network-k", "time-k", "weather-k", "volume-k", "media-k", "battery-k"};
const std::vector<std::string> ModuleLayout = { "1", "2", ";", "6", "3", "4", "5" };
const int EnabledModules[ModulesLength] = { 0, 1, 1, 1, 1, 1 };

// Scale the volume based on individual system - 1 for off
const float VolScaler = 0.865;

const std::string TmpDir = ".tmp"; // Dir to store tmp files in
const std::string HOME = getenv("HOME"); // $HOME environment variable
const std::string WDir = HOME + "/" + "devel/dwmbar-k"; // Working directory
const std::string TDir = WDir + "/" + TmpDir; // Full TmpDir path
const std::string OutputDir = "output"; // Name of output files dir
const std::string ODir = TDir + "/" + OutputDir; // Full OutputDir path
const std::string ClockDir = "clock"; // Name of clock file dir
const std::string CDir = TDir + "/" + ClockDir; // Full ClockDir path
const std::string DataDir = "data"; // Name of data file dir
const std::string DDir = TDir + "/" + DataDir; // Full DataDir path
// Output file store locations
const std::string NetworkOutputFile = ODir + "/network-k.txt";
const std::string TimeOutputFile = ODir + "/time-k.txt";
const std::string WeatherOutputFile = ODir + "/weather-k.txt";
const std::string VolumeOutputFile = ODir + "/volume-k.txt";
const std::string MediaOutputFile = ODir + "/media-k.txt";
const std::string BatteryOutputFile = ODir + "/battery-k.txt";
// Data file store locations
const std::string NetworkDataFile = DDir + "/network.txt";
const std::string TimeDataFile = DDir + "/time.txt";
const std::string WeatherDataFile = DDir + "/weather.txt";
const std::string VolumeDataFile = DDir + "/volume.txt";
const std::string MediaDataFile = DDir + "/media.txt";
const std::string BatteryDataFile = DDir + "/battery.txt";

const int MaxClock = 100; // Maximum clock value before reset
const int ClockFrq = 1; // How many in prog clock cycles before pulse
// How many clock cycles each module runs
const int NetworkFrq = 50;
const int TimeFrq = 1;
const int WeatherFrq = 100;
const int VolumeFrq = 1;
const int MediaFrq = 2;
const int BatteryFrq = 10;

const int SleepTime = 200; // How long while loop waits before re-execution in ms

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

#endif 
