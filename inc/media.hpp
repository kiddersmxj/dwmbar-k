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
    // One player's state, as reported by a single `playerctl -a metadata` call.
    struct Player {
        std::string name;
        std::string status;
        std::string artist;
        std::string title;
        long        position = 0;   // microseconds
    };

    std::vector<Player> GetPlayers();
    std::string FormatPosition(long microseconds);
    std::vector<std::string> splitString(const std::string& str, char delim);
};

REGISTER_MODULE("media", MediaModule)

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
