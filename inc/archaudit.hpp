#pragma once

#include "moduleregistry.hpp"
#include "config.hpp"
#include "module.hpp"

#include <string>

class ArchAuditModule : public Module {
public:
    void run() override;

private:
    struct ArchAuditStatus {
        int count      = -1;  // vulnerable packages
        int upgradable = -1;  // of those, ones with a fix available
        bool valid = false;
    };

    ArchAuditStatus GetStatus();
};

REGISTER_MODULE("archaudit", ArchAuditModule)

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
