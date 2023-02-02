#include <iostream>
#include <cstring>
#include "../include/dwmbar-k.h" 

int main() {
    std::string WttrHex = ExecCmd(R"(timeout 1 curl -s wttr.in/$LOCATION?format=1 | hexdump -v -e '"\\\x" 1/1 "%02x"')", 0, 0);
    if(WttrHex == "") {
        std::cout << "curl did not exec" << std::endl;
        return 1;
    }

    // Allocating space for array, +1 ro allow for NULL term
    char* HexArray = new char[WttrHex.length() + 1];
    strcpy(HexArray, WttrHex.c_str());
    char* substr = strtok(HexArray, R"(\)");

    int i = 0;
    int space = 0;
    std::string HexOutput = "\\";
    while(substr != NULL) {
        if(i < 4 && strcmp(substr, "xef") != 0) {
            HexOutput =+ " \\";
            HexOutput =+ substr;
            std::cout << "\\" << substr;
            /* std::cout << "q"; */
        } else if(strcmp(substr, "x2b") == 0) {
        } else if(strcmp(substr, "x20") == 0 && space == 0) {
            space++;
            /* std::cout << "w"; */
        } else if(space == 1) {
            HexOutput =+"\\";
            HexOutput =+ substr;
            std::cout << "\\" << substr;
        }
        substr = strtok(NULL, R"(\)");
        i++;
    }
    std::cout << std::endl;
    delete[] HexArray;

    return 0;
}

