#include "../include/dwmbar-k.hpp" 
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> SplitString(const std::string& str) {
    std::vector<std::string> tokens;
 
    std::string::size_type pos = 0;
    std::string::size_type prev = 0;
    while ((pos = str.find('\n', prev)) != std::string::npos) {
        std::string tmp = str.substr(prev, pos - prev);
        tokens.push_back(tmp);
        prev = pos + 1;
    }
    tokens.push_back(str.substr(prev));
 
    return tokens;
}

std::vector<std::string> GetPlayers() {
    std::string Players = ExecCmd(R"(playerctl -l)", 0, 0);
    std::vector<std::string> PlayersVector = SplitString(Players);
    PlayersVector.pop_back();
#ifdef MCOUT
    VPrint(PlayersVector);
#endif
    return PlayersVector;
}

std::string GetPlayer() {
    int i = 0;
    std::string Player;
	std::vector<std::string> Players = GetPlayers();
	for(std::string P: Players) {
#ifdef MCOUT
        std::cout << "P-" << P << "-" << std::endl;
#endif
        std::string Output = ExecCmd(R"(playerctl -p )" + P + R"( status)", 0, 0);
        Output.erase(std::remove(Output.begin(), Output.end(), '\n'), Output.cend());
#ifdef MCOUT
        std::cout << "out-" << Output << "-" << std::endl;
#endif
        if(Output == "Playing") {
#ifdef MCOUT
            std::cout << "Playing-" << P << "-" << std::endl;
#endif
            i++;
            Player = P;
		}
	}
	switch(i) {
		case 0:
			return "Paused";
		case 1:
			return Player;
		case 2:
			return "Overloaded";
		default:
			return "Error";
	}
	return "None";
}

void Media() {
    std::string Player = GetPlayer();
	// TODO Add overload to pause old media when new media is played (possibly play again when new media is paused)
    std::cout << "Player=" + Player << std::endl << std::endl;
}

int main() {
    while(1) {
        Media();
    }
    return 0;
}

