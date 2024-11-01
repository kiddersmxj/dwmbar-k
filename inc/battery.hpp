#pragma once
#include "config.hpp"
#include "module.hpp"
#include <string>

class BatteryModule : public Module {
public:
    void run() override;

private:
    std::string TrimWhiteSpace(const std::string& str);
    std::string TrimNewLine(const std::string& str);
    int Battery();

    int Charging = 3;
    std::string BIcon = IBatteryEmpty;
    std::string CIcon = "";
};

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

