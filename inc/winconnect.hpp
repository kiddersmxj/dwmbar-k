#ifndef K_WINCONNECT
#define K_WINCONNECT

#pragma once
#include "moduleregistry.hpp"
#include "config.hpp"
#include "module.hpp"

class WinConnectModule : public Module {
    public:
        void run() override;
    private:
        bool IsWinConnected();
};

#endif

REGISTER_MODULE("winconnect", WinConnectModule)

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

