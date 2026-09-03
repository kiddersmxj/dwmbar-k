#pragma once

#include "moduleregistry.hpp"
#include "config.hpp"
#include "module.hpp"

#include <string>

class CcorralModule : public Module {
public:
    void run() override;

private:
    struct CcorralStatus {
        int  unread    = 0;
        int  open      = 0;
        int  running   = 0;
        int  attention = 0;
        bool muted     = false;
        bool valid     = false;
    };

    CcorralStatus GetStatus();
};

REGISTER_MODULE("ccorral", CcorralModule)

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
