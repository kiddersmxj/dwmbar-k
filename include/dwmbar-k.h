#ifndef dwmbar-k.h 
#define dwmbar-k.h 

#include "config.h"

std::string HOME = getenv("HOME");

std::string ExecCmd(const std::string cmd, const int Output, int ExitStatus);

#endif 

