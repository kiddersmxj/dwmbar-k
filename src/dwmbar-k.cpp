#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include "../include/dwmbar-k.hpp"

void RunModule(std::string Module) {
    std::cout << Module << std::endl;
    std::stringstream LaunchCmd;
    LaunchCmd << "$HOME/devel/dwmbar-k/./launch.sh "<< Module << " &";
    system(LaunchCmd.str().c_str());
}

std::string GetModuleOutput(std::string Module) {
    std::string Output;
    std::ifstream ModuleFile;

    ModuleFile.open(HOME + "/devel/dwmbar-k/.tmp/" + Module + ".txt");

    while(getline(ModuleFile, Output)) {
        std::cout << Output << std::endl;
    }

    ModuleFile.close();
    return Output;
}

std::string ParseModuleNo(char* ModuleNo) {
    std::cout << ModuleNo << " ";
    // -1 to account for 0 based indexing
    return Modules[atoi(ModuleNo) - 1];
}

int main() {
    std::string result = ExecCmd(R"(mkdir $HOME/devel/dwmbar-k/.tmp > /dev/null 2>&1 || echo 1)", 0, 0);
    std::cout << "CMD-" << result << "-" << std::endl;
    if(result == "") {
        std::cout << "mkdir error";
        return 1;
    }

#ifndef NORUN
    // Only runs if .bashrc set $dwmbar to 1
    while(getenv("dwmbar")) {

        for(int i=0; i<ModulesLength; i++) {
            if(ExecCmd("ps -a | grep " + Modules[i], 0, 0) == "") {
                RunModule(Modules[i]);
            }
        }
#endif

        char* ModuleNoArray = new char[ModuleLayout.length() + 1];
        strcpy(ModuleNoArray, ModuleLayout.c_str());
        char* substr = strtok(ModuleNoArray, R"(,)");
    
        int i = 0;
        while(substr != NULL) {
            if(strcmp(substr, ";") == 0) {
                std::cout << ";" << std::endl;
            } else {
                /* std::cout << ParseModuleNo(substr) << std::endl; */
                std::cout << GetModuleOutput(ParseModuleNo(substr)) << std::endl;
            }
            substr = strtok(NULL, R"(,)");
            i++;
        }
        std::cout << std::endl;
        delete[] ModuleNoArray;

        std::cout << "____________" << std::endl;

#ifndef NORUN
    }
#endif

    return 1;
}

