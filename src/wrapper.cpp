#include <std-k.hpp>
#include "../include/config.hpp"

int main(int argc, char** argv) {
    k::ExecCmd(R"(/usr/local/bin/dwmbar-k/dwmbar-k > )" + Logfile + R"( 2>&1 & )", 0 ,0);
}
