#pragma once

#include "moduleregistry.hpp"
#include "config.hpp"
#include "module.hpp"

#include <string>

class ClaudeModule : public Module {
public:
    void run() override;

private:
    struct ClaudeStatus {
        int fiveHour = -1;
        int sevenDay = -1;
        std::string resetTime;
        bool valid = false;
    };

    ClaudeStatus GetStatus();
};

REGISTER_MODULE("claude", ClaudeModule)

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
