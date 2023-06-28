#ifndef Kdwmbar
#define Kdwmbar

#include <thread>
#include <filesystem>
#include <sstream>

#include "config.hpp"

inline int PollClock(const std::string Dir) {
    std::string File;
    int FileInt;
    for (const auto & entry : std::filesystem::directory_iterator(Dir))
        File = std::filesystem::path(entry.path()).filename();
    if(File == "")
        FileInt = -1;
    else
        FileInt = stoi(File);
#ifdef COUT
    std::cout << FileInt << std::endl;
#endif
    return FileInt;
}

using namespace std::chrono_literals;

#endif 

