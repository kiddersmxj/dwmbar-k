#ifndef config.h 
#define config.h 

const int ModulesLength = 5;
const std::string Modules[ModulesLength] = { "network-k", "time-k", "weather-k", "volume-k", "media-k" };
const std::string ModuleLayout = "1,2,;,3,4,5";

const float VolScaler = 0.865;

#endif 
