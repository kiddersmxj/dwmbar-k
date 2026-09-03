#include "../inc/media.hpp"
#include <std-k.hpp>
#include <iostream>
#include <chrono>
#include <thread>

void MediaModule::run() {
    std::string output = " ";
    while (true) {
        std::vector<Player> players = GetPlayers();

        const Player* playing = nullptr;
        for (const Player& p : players) {
            if (p.status == "Playing") { playing = &p; break; }
        }

        // Nothing playing: leave the previous output in place, as before.
        if (playing != nullptr) {
            output = MCol[0] + IPlay + " " + MCol[1] + playing->artist +
                     MCol[2] + " - " + MCol[3] + playing->title +
                     MCol[4] + " (" + MCol[5] + FormatPosition(playing->position) +
                     MCol[4] + ")";

            updateOutput(output);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(MediaSleepTime));
    }
}

// Every player's state in a single call. This used to be five separate
// `playerctl` invocations per tick — list players, status for each, then
// position (piped through sed), artist and title for the playing one — which
// at a 50ms interval was the largest source of forks on the machine.
std::vector<MediaModule::Player> MediaModule::GetPlayers() {
    std::string Raw;
    k::ExecCmd(
        "playerctl -a metadata --format "
        "'{{playerName}}\t{{status}}\t{{artist}}\t{{title}}\t{{position}}' 2>/dev/null",
        Raw);

    std::vector<Player> players;
    for (const std::string& line : splitString(Raw, '\n')) {
        if (line.empty()) continue;

        // Name, status and position are the outer fields and always safe to
        // address positionally; anything extra in the middle is folded back
        // into the title, which is the field most likely to contain a tab.
        std::vector<std::string> f = splitString(line, '\t');
        if (f.size() < 5) continue;

        Player p;
        p.name   = f[0];
        p.status = f[1];
        p.artist = f[2];
        try {
            p.position = std::stol(f.back());
        } catch (...) {
            p.position = 0;             // "No players found", or a player with
        }                               // no position (a stream, say)

        p.title = f[3];
        for (size_t i = 4; i + 1 < f.size(); ++i) p.title += "\t" + f[i];

        players.push_back(p);
    }

#ifdef MCOUT
    for (const Player& p : players)
        std::cout << p.name << " " << p.status << " " << p.title << std::endl;
#endif

    return players;
}

// playerctl reports position in microseconds. Mins wrap at the hour, matching
// the previous behaviour.
std::string MediaModule::FormatPosition(long microseconds) {
    if (microseconds < 0) microseconds = 0;
    long Time = microseconds / 1000000L;
    long Min  = (Time % 3600L) / 60L;
    long Sec  = Time % 60L;

    return std::to_string(Min) + ":" +
           (Sec < 10 ? ("0" + std::to_string(Sec)) : std::to_string(Sec));
}

std::vector<std::string> MediaModule::splitString(const std::string& str, char delim) {
    std::vector<std::string> tokens;
    std::string::size_type pos = 0, prev = 0;

    while ((pos = str.find(delim, prev)) != std::string::npos) {
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
