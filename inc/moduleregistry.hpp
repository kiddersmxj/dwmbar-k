#ifndef K_MODULEREGISTRY
#define K_MODULEREGISTRY

#include "config.hpp"
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class Module;

// Registry of module factories (name → factory)
class ModuleRegistry {
public:
    using Factory = std::function<std::unique_ptr<Module>()>;

    static ModuleRegistry& instance();

    // Register a factory under a given module name
    void registerFactory(const std::string& name, Factory factory);

    // Create a module by name; returns nullptr if not registered
    std::unique_ptr<Module> create(const std::string& name) const;

    // Get a list of all registered module names (for debugging/inspection)
    std::vector<std::string> availableModules() const;

private:
    ModuleRegistry() = default;
    ModuleRegistry(const ModuleRegistry&) = delete;
    ModuleRegistry& operator=(const ModuleRegistry&) = delete;

    std::unordered_map<std::string, Factory> factories;
};

// Macro used in each module's .cpp to self-register
#define REGISTER_MODULE(moduleName, ModuleType)                                      \
    namespace {                                                                      \
        struct ModuleType##_registrar {                                              \
            ModuleType##_registrar() {                                               \
                ModuleRegistry::instance().registerFactory(                          \
                    moduleName,                                                      \
                    []() -> std::unique_ptr<Module> {                               \
                        return std::make_unique<ModuleType>();                       \
                    }                                                                \
                );                                                                   \
            }                                                                        \
        };                                                                           \
        static ModuleType##_registrar global_##ModuleType##_registrar;               \
    }

#endif

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

