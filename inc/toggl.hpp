#ifndef K_TOGGL
#define K_TOGGL

#pragma once
#include "moduleregistry.hpp"
#include "config.hpp"
#include "module.hpp"

class TogglModule : public Module {
    public:
        void run() override;
    private:
        int Cooldown = 0;

        struct TogglStatus {
            std::string id;
            std::string status;
            std::string duration;
            std::string project;
            std::string description;
            std::string tags;
            long durationSeconds;
            std::chrono::system_clock::time_point fetchedAt;
        };

        bool IsRunning(TogglStatus& status);
        std::string GetDuration(TogglStatus& status);
        TogglStatus GetStatus();
};

#endif

REGISTER_MODULE("toggl", TogglModule)

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

