#include <iostream>
#include "../include/dwmbar-k.hpp" 

int main() {
    while(1) {
        ExecCmd(R"(nmcli -v)", 0, 0);
    }

    return 1;
}

