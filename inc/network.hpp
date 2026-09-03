#pragma once

#include "moduleregistry.hpp"
#include "config.hpp"
#include "module.hpp"

class NetworkModule : public Module {
public:
    void run() override;

private:
    // iface is passed in rather than re-resolved: the caller has just done
    // that work, and resolving costs a shell script full of forks.
    std::string getConnectionName(const std::string& iface);
    std::string getPublicIP();
    std::string getPrivateIP();
};

REGISTER_MODULE("network", NetworkModule)

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

