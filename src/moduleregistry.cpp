#include "../inc/moduleregistry.hpp"
#include "../inc/module.hpp"

#include <utility>

ModuleRegistry& ModuleRegistry::instance() {
    static ModuleRegistry inst;
    return inst;
}

void ModuleRegistry::registerFactory(const std::string& name, Factory factory) {
    factories[name] = std::move(factory);
}

std::unique_ptr<Module> ModuleRegistry::create(const std::string& name) const {
    auto it = factories.find(name);
    if (it == factories.end()) {
        return nullptr;
    }
    return (it->second)();
}

std::vector<std::string> ModuleRegistry::availableModules() const {
    std::vector<std::string> names;
    names.reserve(factories.size());
    for (const auto& kv : factories) {
        names.push_back(kv.first);
    }
    return names;
}

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

