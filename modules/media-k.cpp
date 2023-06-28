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
    std::string Players = k::ExecCmd(R"(playerctl -l)", 0, 0);
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
#ifdef MediaDCOUT
        std::cout << "P-" << P << "-" << std::endl;
#endif
        std::string Output = k::StripTrailingNL(k::ExecCmd(R"(playerctl -p )" + P + R"( status)", 0, 0));
#ifdef MediaDCOUT
        std::cout << "out-" << Output << "-" << std::endl;
#endif
        if(Output == "Playing") {
#ifdef MediaDCOUT
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

std::string GetTimeFromStart() {
	int Time, Sec, Min;
	Time = stoi(k::ExecCmd(R"(playerctl position | sed 's/..\{6\}$//')", 0, 0));
	Min = (Time % 3600) / 60;
	Sec = Time % 60;
#ifdef MediaDCOUT
	std::cout << "min=" << Min << " ";
	std::cout << "sec=" << Sec << std::endl;
#endif
	// Return value in Min:Sec format - ternary used to make sure signe digit integers have zero before for formatting
	return std::to_string(Min) + ":" + (Sec < 10 ? ("0" + std::to_string(Sec)) : std::to_string(Sec));
}

std::string GetArtist(std::string Player) {
    return k::StripTrailingNL(k::ExecCmd(R"(playerctl --player=)" + Player + R"( metadata --format '{{ artist }}')", 0 ,0));
}

std::string GetTitle(std::string Player) {
    return k::StripTrailingNL(k::ExecCmd(R"(playerctl --player=)" + Player + R"( metadata --format '{{ title }}')", 0, 0));
}

void Media(std::string &Out) {
    if(!Run())
        return;
#ifdef MediaDCOUT
    std::cout << std::endl;
#endif
    media.Player = GetPlayer();
    if(media.Player != "Paused") {
        media.Hex = IPlay;
        media.Artist = GetArtist(media.Player);
        media.Title = GetTitle(media.Player);
		media.Time = GetTimeFromStart();
    } else if (media.Player == "Overload") {
	    // TODO add overload to pause old media when new media is played (possibly play again when new media is paused) - togglable?
    } else {
        media.Hex = IPause;
    }

#ifdef MediaMCOUT
    std::cout << R"(Player=")" << media.Player << R"(")" << std::endl;
    std::cout << R"(Artist=")" << media.Artist << R"(")" << std::endl;
    std::cout << R"(Title=")" << media.Title << R"(")" << std::endl << std::endl;
    std::cout << R"(Time=")" << media.Time << R"(")" << std::endl << std::endl;
#endif

    std::vector<std::string> Output;
    std::string O;
    if(media.Title == "") {
        O = " ";
        if(Out != O) {
            Output.push_back(" ");
            Out = O;
        }
        return;
    }

    O = MCol[0] + media.Hex + " " + MCol[1] + media.Artist + MCol[2] + " - " + MCol[3] + media.Title + MCol[4] + " (" + MCol[5] + media.Time + MCol[4] + ")";

    if(Out != O) {
        Output.push_back(R"($(printf ")" + O + R"("))");
        Out = O;
        k::WriteFileLines(Output, MediaOutputFile);
    }
}

int main() {
    std::string Out = " ";
    while(1) {
        Media(Out);
        std::this_thread::sleep_for(std::chrono::milliseconds(SleepTime));
        k::BreakPoint();
    }
    return 0;
}

