#include <iostream>
#include "../include/dwmbar-k.h" 

void RunSector(std::string Sector) {
    std::cout << "1:" + Sector << std::endl;
}

int main() {
    while(1) {
        for(int i=0; i<SectorsLength; i++) {
            if(ExecCmd("ps -a | grep " + Sectors[i], 0, 0) == "") {
                RunSector(Sectors[i]);
        }

        }
    }
}

