#include "../include/dwmbar-k.hpp" 
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

int C = -1;

struct {
    std::string Player, Hex, Artist, Title, Time;
} media;

int Run() {
    if(C == PollClock(CDir)) {
        return 0;
    }
    C = PollClock(CDir);
    if((PollClock(CDir) % MediaFrq) == 0 || PollClock(CDir) == 0)
        return 1;
    return 0; 
}

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
        std::string Output = StripTrailingNL(ExecCmd(R"(playerctl -p )" + P + R"( status)", 0, 0));
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

std::string GetArtist(std::string Player) {
    return StripTrailingNL(ExecCmd(R"(playerctl --player=)" + Player + R"( metadata --format '{{ artist }}')", 0 ,0));
}

std::string GetTitle(std::string Player) {
    return StripTrailingNL(ExecCmd(R"(playerctl --player=)" + Player + R"( metadata --format '{{ title }}')", 0, 0));
}

void Media() {
    if(!Run())
        return;
    std::cout << std::endl;
    media.Player = GetPlayer();
	// TODO Add overload to pause old media when new media is played (possibly play again when new media is paused) - togglable?
    if(media.Player != "Paused") {
        media.Hex = IPlay;
        media.Artist = GetArtist(media.Player);
        media.Title = GetTitle(media.Player);
    } else if (media.Player == "Overload") {
        // TODO overload error
    } else {
        media.Hex = IPause;
    }

#ifdef MCOUT
    std::cout << R"(Player=")" << media.Player << R"(")" << std::endl;
    std::cout << R"(Artist=")" << media.Artist << R"(")" << std::endl;
    std::cout << R"(Title=")" << media.Title << R"(")" << std::endl << std::endl;
#endif

    std::string Out = media.Hex + " " + media.Artist + " - " + media.Title;

    std::vector<std::string> Output;
    Output.push_back(R"($(printf ")" + Out + R"("))");
    WriteFileLines(Output, MediaOutputFile);
}

int main() {
    while(1) {
        Media();
        BreakPoint();
    }
    return 0;
}

