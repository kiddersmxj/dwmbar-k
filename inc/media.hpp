#pragma once
#include "moduleregistry.hpp"
#include "config.hpp"
#include "module.hpp"
#include <string>
#include <vector>

class MediaModule : public Module {
public:
    void run() override;

private:
    std::vector<std::string> splitString(const std::string& str);
    std::vector<std::string> GetPlayers();
    std::string GetPlayer();
    std::string GetTimeFromStart();
    std::string GetArtist(std::string player);
    std::string GetTitle(std::string player);
    bool runCondition();
};

REGISTER_MODULE("media", MediaModule)

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

