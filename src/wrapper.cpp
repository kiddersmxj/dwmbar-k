#include <std-k.hpp>
#include "../inc/config.hpp"

int main(int argc, char** argv) {
    k::ExecCmd(R"(pkill dwmbar; /usr/local/bin/dwmbarsrc/dwmbarsrc > )" + Logfile + R"( 2>&1 & )");
}
