#ifndef config.h 
#define config.h 

const int ModulesLength = 5;
const std::string Modules[ModulesLength] = { "network-k", "time-k", "weather-k", "volume-k", "media-k" };
const std::string ModuleLayout = "1,2,;,3,4,5";

const float VolScaler = 0.865;

const std::string IBatteryFull = R"(\xef\x89\x80)";
const std::string IBatteryThreeQuart = R"(\xef\x89\x81)";
const std::string IBatteryHalf = R"(\xef\x89\x82)";
const std::string IBatteryQuart = R"(\xef\x89\x83)";
const std::string IBatteryEmpty = R"(\xef\x89\x84)";
const std::string ICharging = R"(\xef\x83\xa7)";

const int BFull = 100;
const int BThreeQuart = 75;
const int BHalf = 50;
const int BQuart = 25;
const int BEmpty = 10;

#endif 
