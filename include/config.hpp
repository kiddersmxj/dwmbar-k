#ifndef Kconfig
#define Kconfig

const int ModulesLength = 6;
const std::string Modules[ModulesLength] = { "network-k", "time-k", "weather-k", "volume-k", "media-k", "battery-k"};
const std::vector<std::string> ModuleLayout = { "1", "2", ";", "6", "3", "4", "5" };

// Scale the volume based on individual system - 1 for off
const float VolScaler = 0.865;

const std::string TmpDir = ".tmp"; // Dir to store tmp files in
const std::string HOME = getenv("HOME"); // $HOME environment variable
const std::string WDir = HOME + "/" + "devel/dwmbar-k"; // Working directory
const std::string TDir = WDir + "/" + TmpDir; // Full TmpDir path
// Output file store locations
const std::string NetworkOutputFile = TDir + "/network-k.txt";
const std::string TimeOutputFile = TDir + "/time-k.txt";
const std::string WeatherOutputFile = TDir + "/weather-k.txt";
const std::string VolumeOutputFile = TDir + "/volume-k.txt";
const std::string MediaOutputFile = TDir + "/media-k.txt";
const std::string BatteryOutputFile = TDir + "/battery-k.txt";

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

#endif 
