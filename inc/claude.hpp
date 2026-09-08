#pragma once

#include "moduleregistry.hpp"
#include "config.hpp"
#include "module.hpp"

#include <string>

class ClaudeModule : public Module {
public:
    void run() override;

private:
    // What the bar actually knows. Fresh and Stale both carry real figures;
    // NoReading carries none, and must never be rendered as a zero.
    enum class State { NoReading, Stale, Fresh };

    struct ClaudeStatus {
        State state = State::NoReading;
        int fiveHour = -1;
        int sevenDay = -1;
        std::string resetTime;   // HH:MM local — when the 5-hour window resets
        std::string polledTime;  // HH:MM local — when bursar last got a figure
    };

    ClaudeStatus GetStatus();
};

REGISTER_MODULE("claude", ClaudeModule)

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
