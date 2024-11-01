#include "config.hpp"
#include "module.hpp"

#pragma once

class NetworkModule : public Module {
public:
    void run() override;
private:
    std::string getConnectionName();
    std::string getPublicIP();
    std::string getPrivateIP();
};

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

