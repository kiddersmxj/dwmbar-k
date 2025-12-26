#pragma once
#include "moduleregistry.hpp"
#include "module.hpp"
#include <string>

class WeatherModule : public Module {
public:
    void run() override;

private:
    std::string GetWeather();
};

REGISTER_MODULE("weather", WeatherModule)

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

