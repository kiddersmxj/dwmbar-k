#include "../inc/dwmbar.hpp"
#include <X11/Xlib.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <thread>
#include <iostream>
#include <vector>
#include <unordered_set>

int main() {
    initializeConfig(ConfigFile);

    ModuleRegistry& registry = ModuleRegistry::instance();

    // Build list of modules to actually run, based on ModuleLayout
    std::vector<ModuleInfo> modules;
    modules.reserve(ModuleLayout.size());

    std::unordered_set<std::string> seenNames;

    for (const auto& name : ModuleLayout) {
        if (name == Separator) {
            continue; // not a module name
        }

        // Avoid starting multiple threads for the same module name
        if (!seenNames.insert(name).second) {
            continue; // already instantiated
        }

        std::unique_ptr<Module> instance = registry.create(name);
        if (!instance) {
            std::cerr << "Warning: unknown module name in layout: " << name << '\n';
            continue;
        }

        Module* rawPtr = instance.get();

        ModuleInfo info;
        info.name = name;
        info.module = std::move(instance);
        info.thread = std::thread(&Module::run, rawPtr);

        modules.push_back(std::move(info));
    }

    // Main loop
    while (true) {
        // 1. Collect outputs
        std::vector<std::pair<std::string, std::string>> outputVector;
        outputVector.reserve(modules.size());

        for (auto& info : modules) {
            Module* mod = info.module.get();
            std::string output;

            std::unique_lock<std::mutex> lock(mod->getOutputMutex());
            if (mod->getOutputCV().wait_for(
                    lock,
                    std::chrono::milliseconds(1),
                    [&] { return !mod->getOutputNoLock().empty(); })) {

                output = mod->getOutputNoLock();
                if (output == "NaN") {
                    output.clear();
                }
                outputVector.emplace_back(info.name, output);
            }
        }

        // 2. Order according to ModuleLayout (with separators)
        std::vector<std::string> OrderedVector;
        OrderedVector.reserve(ModuleLayout.size());

        for (const auto& moduleName : ModuleLayout) {
            if (moduleName == Separator) {
                OrderedVector.push_back(Separator);
            } else {
                auto it = std::find_if(
                    outputVector.begin(),
                    outputVector.end(),
                    [&](const std::pair<std::string, std::string>& p) {
                        return p.first == moduleName;
                    }
                );

                if (it != outputVector.end()) {
                    OrderedVector.push_back(it->second);
                }
                // If a module produced nothing in this tick, it is just omitted.
            }
        }

        // 3. Render to X root window
        XSR(ParseXSR(OrderedVector));

        std::this_thread::sleep_for(std::chrono::milliseconds(SleepTime));
    }

    // If you ever add a proper shutdown, you would break the loop and then:
    // for (auto& info : modules) {
    //     if (info.thread.joinable()) {
    //         info.thread.join();
    //     }
    // }

    return 0;
}

std::string ParseXSR(std::vector<std::string> OutputVector) {
	std::string XSRBody = "";
    std::vector<int> Empty;
    bool Escape = 1;
    int i = 0;

    // Make sure previous Output was not empty or BarDelimeter
    for(std::string Output: OutputVector) {
        if(Output != " " && Output != "" && Output != BarDelimeter)
            Empty.push_back(1);
        else
            Empty.push_back(0);
    }

    for(int I: Empty) {
        if(I == 1) {
            if(Escape == 0)
                XSRBody += " " + ModuleDelimeter;
            else
                Escape = 0;
        } else if(OutputVector.at(i) == BarDelimeter)
            Escape = 1;
        if(OutputVector.at(i) != " " && OutputVector.at(i) != "")
		    XSRBody += " " + OutputVector.at(i);
        i++;
    }

	return XSRBody;
}

// Function to decode hex-encoded sequences
void decode_hex_string(const char *input, char *output) {
    while (*input) {
        if (*input == '\\' && *(input + 1) == 'x') {
            // Convert the hex pair to a character
            char hex[3] = { *(input + 2), *(input + 3), '\0' };
            *output++ = (char)strtol(hex, NULL, 16);
            input += 4; // Move past the hex sequence
        } else {
            *output++ = *input++;
        }
    }
    *output = '\0'; // Null-terminate the output string
}

// Function to set the root window name with a passed status string
void XSR(const std::string &raw_status) {
    Display *display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Unable to open X display\n");
        return;
    }

    Window root = DefaultRootWindow(display);

    // Decode the status string
    char decoded_status[2048];
    decode_hex_string(raw_status.c_str(), decoded_status);

    // Set the root window name
    XStoreName(display, root, decoded_status);
    XFlush(display);

    // Close the connection to the X server
    XCloseDisplay(display);
}

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

