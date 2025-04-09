#include "config.hpp"

#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#pragma once

class Module {
public:
    virtual ~Module() = default;
    virtual void run() = 0;

    // Getter for output (locks the mutex)
    std::string getOutput();

    // Getter for output without locking (mutex must already be locked)
    std::string getOutputNoLock() const;

    // Getters for mutex and condition variable
    std::mutex& getOutputMutex() { return output_mutex; }
    std::condition_variable& getOutputCV() { return output_cv; }

protected:
    void updateOutput(const std::string& new_output);

    std::string output;
    std::mutex output_mutex;
    std::condition_variable output_cv;
};

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

