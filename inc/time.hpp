#pragma once

#include "config.hpp"

#include "moduleregistry.hpp"
#include "module.hpp"
#include <string>
#include <vector>

class TimeModule : public Module {
public:
    void run() override;

private:
    std::string GetTime();
    std::string GetDay(std::string day);
    std::vector<std::string> GetDate();
};

REGISTER_MODULE("time", TimeModule)

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

