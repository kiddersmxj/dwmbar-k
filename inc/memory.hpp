#ifndef K_MEMORY
#define K_MEMORY

#include "config.hpp"
#include "module.hpp"
#include <string>

#pragma once

class MemoryModule : public Module {
public:
    void run() override;
private:
    std::string getMemory();
    std::string getSwap();
};



#endif

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

