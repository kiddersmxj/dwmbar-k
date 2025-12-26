#pragma once

#include "moduleregistry.hpp"
#include "config.hpp"
#include "module.hpp"
#include <string>

class VolumeModule : public Module {
public:
    void run() override;

private:
    int GetVolumeLevel(char *device, char *selemName);
};

REGISTER_MODULE("volume", VolumeModule)

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

