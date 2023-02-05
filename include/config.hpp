#ifndef config
#define config

const int ModulesLength = 6;
const std::string Modules[ModulesLength] = { "network-k", "time-k", "weather-k", "volume-k", "media-k", "battery-k"};
const std::string ModuleLayout = "1,2,;,6,3,4,5";

const float VolScaler = 0.865;

const int BFull = 90;
const int BThreeQuart = 75;
const int BHalf = 50;
const int BQuart = 25;
const int BEmpty = 10;

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
