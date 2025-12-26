#ifndef K_STORAGE
#define K_STORAGE

#pragma once

#include "moduleregistry.hpp"
#include "config.hpp"
#include "module.hpp"
#include <string>

class StorageModule : public Module {
public:
    void run() override;
private:
    std::string getStorage();
};


#endif

REGISTER_MODULE("storage", StorageModule)

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

