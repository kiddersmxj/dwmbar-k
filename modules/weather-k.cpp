#include <iostream>
#include <cstring>
#include "../include/dwmbar-k.hpp" 

int C = -1;

int Run() {
    if(C == PollClock(CDir)) {
        return 0;
    }
    C = PollClock(CDir);
    if((PollClock(CDir) % WeatherFrq) == 0 || PollClock(CDir) == 0)
        return 1;
    return 0; 
}

int Weather() {
    if(!Run())
        return 1;
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
    std::string HexOutput = "";
    while(substr != NULL) {
        if(strcmp(substr, "x0a") == 0) {
        } else if(i < 4 && strcmp(substr, "xef") != 0) {
            HexOutput += "\\";
            HexOutput += substr;
            /* std::cout << "\\" << substr; */
            /* std::cout << "q"; */
        } else if(strcmp(substr, "x2b") == 0) {
        } else if(strcmp(substr, "x20") == 0 && space == 0) {
            space++;
            /* std::cout << "w"; */
        } else if(space == 1) {
            HexOutput += "\\";
            HexOutput += substr;
            /* std::cout << "\\" << substr; */
        }
        substr = strtok(NULL, R"(\)");
        i++;
    }
#ifdef MCOUT
    std::cout << std::endl;
#endif
    delete[] HexArray;

    std::vector<std::string> Output;
    Output.push_back(HexOutput);

    WriteFileLines(Output, WeatherOutputFile);

#ifdef MCOUT
    VPrint(Output);
#endif
    return 0;
}


int main() {
    while(1) {
        Weather();
        std::this_thread::sleep_for(std::chrono::milliseconds(SleepTime));
    }
    return 0;
}

