#include "../inc/module.hpp"

std::string Module::getOutput() {
    std::lock_guard<std::mutex> lock(output_mutex);
    return output;
}

std::string Module::getOutputNoLock() const {
    return output;
}

void Module::updateOutput(const std::string& new_output) {
    // std::cout << "Module: output updated to " << new_output << std::endl;
    {
        std::lock_guard<std::mutex> lock(output_mutex);
        output = new_output;
    }
    output_cv.notify_one();
}

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 
