#include <iostream>
#include "../include/dwmbar-k.h" 

void RunSector(std::string Sector) {
    std::cout << Sector << std::endl;
    ExecCmd("$HOME/devel/dwmbar-k/./launch.sh " + Sector, 0, 0);
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

