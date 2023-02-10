#include <iostream>
#include "../include/dwmbar-k.hpp" 

int main() {
    while(1) {
        /* ExecCmd(R"(playerctl -l | awk '{gsub(/[[:punct:]]/, "\n")} 1' RS='[[:space:]]')", 0, 0); */
        ExecCmd(R"(playerctl -v)", 0, 0);
    }

    return 1;
}

