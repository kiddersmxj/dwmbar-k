#include "../inc/media.hpp"
#include <std-k.hpp>
#include <iostream>
#include <chrono>
#include <thread>

void MediaModule::run() {
    std::string output = " ";
    while (true) {
        std::string player = GetPlayer();
        std::string artist, title, time;

        if (player != "Paused") {
            artist = GetArtist(player);
            title = GetTitle(player);
            time = GetTimeFromStart();

            output = R"($(printf ")" + MCol[0] + IPlay + " " + MCol[1] + artist + MCol[2] + " - " + MCol[3] + title + MCol[4] + " (" + MCol[5] + time + MCol[4] + ")" + R"("))";

            updateOutput(output);
        } else {
            // updateOutput("Paused");
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(MediaSleepTime));
    }
}


std::vector<std::string> MediaModule::GetPlayers() {
    std::string Players;
    k::ExecCmd(R"(playerctl -l)", Players);
    std::vector<std::string> PlayersVector = splitString(Players);
    PlayersVector.pop_back();
#ifdef MCOUT
    VPrint(PlayersVector);
#endif
    return PlayersVector;
}

// std::string MediaModule::GetPlayer() {
//     std::vector<std::string> players = splitString(k::ExecCmd("playerctl -l"));
//     for (const auto& player : players) {
//         std::string status = k::ExecCmd("playerctl -p " + player + " status");
//         if (status == "Playing") return player;
//     }
//     return "Paused";
// }

std::string MediaModule::GetPlayer() {
    int i = 0;
    std::string Player;
	std::vector<std::string> Players = GetPlayers();
	for(std::string P: Players) {
        std::string Output;
        k::ExecCmd(R"(playerctl -p )" + P + R"( status)", Output);
        Output = k::StripTrailingNL(Output);
        if(Output == "Playing") {
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

std::string MediaModule::GetTimeFromStart() {
	int Time, Sec, Min;
    std::string Output;
    k::ExecCmd(R"(playerctl position | sed 's/..\{6\}$//')", Output);
	Time = stoi(Output);
	Min = (Time % 3600) / 60;
	Sec = Time % 60;

	// Return value in Min:Sec format - ternary used to make sure signe digit integers have zero before for formatting
	return std::to_string(Min) + ":" + (Sec < 10 ? ("0" + std::to_string(Sec)) : std::to_string(Sec));
}

std::string MediaModule::GetArtist(std::string Player) {
    std::string Output;
    k::ExecCmd(R"(playerctl --player=)" + Player + R"( metadata --format '{{ artist }}')", Output);
    return k::StripTrailingNL(Output);
}

std::string MediaModule::GetTitle(std::string Player) {
    std::string Output;
    k::ExecCmd(R"(playerctl --player=)" + Player + R"( metadata --format '{{ title }}')", Output);
    return k::StripTrailingNL(Output);
}

// Define splitString to split strings by newline
std::vector<std::string> MediaModule::splitString(const std::string& str) {
    std::vector<std::string> tokens;
    std::string::size_type pos = 0, prev = 0;

    while ((pos = str.find('\n', prev)) != std::string::npos) {
        tokens.push_back(str.substr(prev, pos - prev));
        prev = pos + 1;
    }
    tokens.push_back(str.substr(prev));  // Add the final token
    return tokens;
}

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

