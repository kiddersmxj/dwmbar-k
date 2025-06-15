#ifndef K_STORAGE
#define K_STORAGE

#include "config.hpp"
#include "module.hpp"
#include <string>

#pragma once

class StorageModule : public Module {
public:
    void run() override;
private:
    std::string getStorage();
};


#endif

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

