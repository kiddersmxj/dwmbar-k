#include <iostream>
#include <sstream>
#include "../include/dwmbar-k.h" 

void RunSector(std::string Sector) {
    std::cout << Sector << std::endl;
    std::stringstream stream;
    stream << "$HOME/devel/dwmbar-k/./launch.sh "<< Sector << " &";
    system(stream.str().c_str());
}

int main() {
    while(1) {
        for(int i=0; i<SectorsLength; i++) {
            if(ExecCmd("ps -a | grep " + Sectors[i], 0, 0) == "") {
                RunSector(Sectors[i]);
            }
        }
    }

    return 1;
}

