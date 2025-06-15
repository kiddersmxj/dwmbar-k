#include "../inc/storage.hpp"

void StorageModule::run() {
    std::string output = " ";
    while (true) {
        std::string Storage = getStorage();

        if (!Storage.empty()) {
            output = StrCol[0] + IHdd + " " + StrCol[1] + Storage + "%";
            // std::cout << "StorageModule: Updating output to " << output << std::endl;  // Debug
            updateOutput(output);
        } else {
            output = NoOutputCode;
            // std::cout << "StorageModule: No output" << std::endl;  // Debug
            updateOutput(output);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(StorageSleepTime));
    }
}

std::string StorageModule::getStorage() {
    std::string Storage;
    k::ExecCmd(R"(df -h / | awk 'NR==2 {print $5}' | tr -d '%')", Storage);
    Storage = k::StripTrailingNL(Storage);
    return Storage;
}

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

