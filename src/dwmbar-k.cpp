#include <iostream>
#include <sstream>
#include "../include/dwmbar-k.h" 

void RunModule(std::string Module) {
    std::cout << Module << std::endl;
    std::stringstream LaunchCmd;
    LaunchCmd << "$HOME/devel/dwmbar-k/./launch.sh "<< Module << " &";
    system(LaunchCmd.str().c_str());
}

void ParseModulePositions(char Positions) {
    Positions = "skrs";
}

int main() {
    char* Positions;
    ParseModulePositions(*Positions);
    for(int i; i<ModulesLength; i++) {
        if(Positions[i] != NULL) {
            std::cout << Positions[i];
        }
    }
    while(1) {
        for(int i=0; i<ModulesLength; i++) {
            if(ExecCmd("ps -a | grep " + Modules[i], 0, 0) == "") {
                RunModule(Modules[i]);
            }
        }


    }

    return 1;
}

