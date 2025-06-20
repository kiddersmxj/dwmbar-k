#ifndef K_DWMBAR
#define K_DWMBAR

#include <thread>
#include <filesystem>
#include <sstream>
#include <algorithm>

#include "network.hpp"
#include "bluetooth.hpp"
#include "media.hpp"
#include "time.hpp"
#include "volume.hpp"
#include "weather.hpp"
#include "battery.hpp"
#include "signal.hpp"
#include "cpu.hpp"
#include "throttling.hpp"
#include "brightness.hpp"
#include "winconnect.hpp"
#include "chronolog.hpp"
#include "memory.hpp"
#include "storage.hpp"
#include "config.hpp"


std::string ParseModuleNo(std::string ModuleNo);
// void XSR(std::string Body);
void decode_hex_string(const char *input, char *output);
void XSR(const std::string &raw_status);
std::string ParseXSR(std::vector<std::string> VectorOutput);
int GetIndex(std::vector<std::string> v, std::string K);
int Enabled(int i);

#endif

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

