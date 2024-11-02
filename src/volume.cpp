#include "../inc/volume.hpp"

#include <alsa/asoundlib.h>
#include <iostream>
#include <chrono>
#include <thread>

const snd_mixer_selem_channel_id_t CHANNEL = SND_MIXER_SCHN_FRONT_LEFT;
int err;
snd_mixer_t *h_mixer;
snd_mixer_selem_id_t *sid;
snd_mixer_elem_t *elem;
std::string VIcon = "";
char *device = "default";
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
            Level = k::Map(Level, 0, 65535, 0, 100);
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
    long vol;
    if ((err = snd_mixer_open(&h_mixer, 1)) < 0)
        return -1;

    if ((err = snd_mixer_attach(h_mixer, device)) < 0)
        return -1;

    if ((err = snd_mixer_selem_register(h_mixer, NULL, NULL)) < 0)
        return -1;

    if ((err = snd_mixer_load(h_mixer)) < 0)
        return -1;

    snd_mixer_selem_id_alloca(&sid);
    snd_mixer_selem_id_set_index(sid, 0);
    snd_mixer_selem_id_set_name(sid, selem_name);

    if ((elem = snd_mixer_find_selem(h_mixer, sid)) == NULL) {
        snd_mixer_close(h_mixer);
        return -1;
    }

    snd_mixer_selem_get_playback_volume(elem, CHANNEL, &vol);
    snd_mixer_close(h_mixer);
    vol = round(static_cast<float>(vol) * (1 / VolScaler));

    return vol;
}

// Copyright (c) 2024, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 

