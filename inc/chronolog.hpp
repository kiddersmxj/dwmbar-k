#ifndef K_CHRONOLOG
#define K_CHRONOLOG

#pragma once
#include "config.hpp"
#include "module.hpp"

class ChronologModule : public Module {
    public:
        void run() override;
    private:
        bool IsRunning(std::string Name);
        bool StringToBool(const std::string &input);
        std::string GetElapsed(std::string Name);
};

#endif

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

