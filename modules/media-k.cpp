#include <iostream>
#include "../include/dwmbar-k.h" 

int main() {
    while(1) {
        ExecCmd("ps -a", 0, 0);
    }
    
    return 1;
}

