#ifndef K_MEMORY
#define K_MEMORY

#pragma once
#include "moduleregistry.hpp"
#include "config.hpp"
#include "module.hpp"
#include <string>

class MemoryModule : public Module {
public:
    void run() override;
private:
    // RAM and swap utilisation, as whole percentages. Mirrors what procps
    // `free` reports: used = MemTotal - MemAvailable (its definition since
    // 3.3.10), swap used = SwapTotal - SwapFree.
    struct Usage {
        int  mem   = 0;
        int  swap  = 0;
        bool valid = false;
    };
    Usage getUsage();
};



#endif

REGISTER_MODULE("memory", MemoryModule)

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
