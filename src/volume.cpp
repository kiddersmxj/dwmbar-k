#include "../inc/volume.hpp"

// Switched from ALSA mixer API to wpctl so the bar correctly reflects
// PipeWire software-boost volumes above 100 % (MAX_VOL > 1.0).
#include <cstdio>   // FILE, popen, pclose, fgets, sscanf
#include <cstring>  // strstr
#include <cmath>    // round
#include <iostream>
#include <chrono>
#include <thread>

std::string VIcon = "";
char *device = "default";  // unused; kept so call-site in run() compiles unchanged
int C = -1;

void VolumeModule::run() {
    while (true) {
        char *selem_name = "Master";
        int Level = GetVolumeLevel(device, selem_name);
        if (Level == -1) {
            std::cerr << "VolumeModule: Error" << std::endl;
        }

        auto placeholder = "Master";
        if (selem_name == placeholder) {
            // VolDisplayMax = MAX_VOL * 100 (see config.hpp); maps 0–MAX_VOL → 0–100 %
            Level = k::Map(Level, 0, VolDisplayMax, 0, 100);
        }
        std::string Out;

        if (Level >= 0) {
            std::string Icon = (Level >= VHigh) ? IVolHigh : (Level >= VMid) ? IVolMid : (Level >= VLow) ? IVolLow : IVolMute;
            Out = VCol[0] + Icon + " " + VCol[1] + std::to_string(Level) + "%";
        } else {
            Out = "Muted";
        }

        updateOutput(Out);
        std::this_thread::sleep_for(std::chrono::milliseconds(VolumeSleepTime));
    }
}

int VolumeModule::GetVolumeLevel(char *device, char *selem_name) {
    // device and selem_name are unused; kept for API compatibility with run().
    // Read the actual PipeWire sink volume via wpctl so values above 1.0
    // (software boost) are captured correctly.
    FILE *fp = popen("wpctl get-volume @DEFAULT_AUDIO_SINK@ 2>/dev/null", "r");
    if (!fp) return -1;
    char buf[64] = {0};
    fgets(buf, sizeof(buf), fp);
    pclose(fp);

    // wpctl prints "[MUTED]" when muted — signal -1 so caller shows "Muted"
    if (strstr(buf, "MUTED")) return -1;

    float vol = 0.0f;
    if (sscanf(buf, "Volume: %f", &vol) != 1) return -1;

    // Return vol * 100 so k::Map(result, 0, VolDisplayMax, 0, 100) gives the
    // correct display percentage (e.g. 2.0 → 200 → 100 %, 1.0 → 100 → 50 %).
    return static_cast<int>(round(vol * 100.0f));
}

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

