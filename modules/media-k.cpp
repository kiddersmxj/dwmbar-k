#include <iostream>
#include "../include/dwmbar-k.h" 

int main() {
    while(1) {
        ExecCmd(R"(playerctl -l | awk '{gsub(/[[:punct:]]/, "\n")} 1' RS='[[:space:]]')", 0, 0);
    }

    return 1;
}

