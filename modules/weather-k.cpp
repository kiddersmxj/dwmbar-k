#include "../include/dwmbar-k.hpp" 

int C = -1;

int Run() {
    int Clk = PollClock(CDir);
    if(C == Clk) {
        return 0;
    }
    C = Clk;
    if((Clk % WeatherFrq) == 0 || Clk == 0)
        return 1;
    return 0; 
}

int Weather(std::string &Out) {
    if(!Run())
        return 1;
    std::string WttrHex;
    k::ExecCmd(R"(timeout 1 curl -s "wttr.in/$city?format=1&m" | hexdump -v -e '"\\\x" 1/1 "%02x"')", WttrHex);
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
#ifdef WeaMCOUT
    std::cout << std::endl;
#endif
    delete[] HexArray;

    std::vector<std::string> Output;
    Output.push_back(R"($(printf ")" + WCol[0] + HexOutput + BDCol + R"("))");

    if(Out != Output.at(0))
        k::WriteFileLines(Output, WeatherOutputFile);
    Out = Output.at(0);

#ifdef WeaMCOUT
    VPrint(Output);
#endif
    return 0;
}


int main() {
    std::string Out = " ";
    while(1) {
        Weather(Out);
        std::this_thread::sleep_for(std::chrono::milliseconds(SleepTime));
    }
    return 0;
}

