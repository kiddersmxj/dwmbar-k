#include <cmath>
#include <alsa/asoundlib.h>
#include <cstdio>
#include "../include/dwmbar-k.hpp" 
using namespace std::chrono_literals;

const snd_mixer_selem_channel_id_t CHANNEL = SND_MIXER_SCHN_FRONT_LEFT;
int err;
snd_mixer_t *h_mixer;
snd_mixer_selem_id_t *sid;
snd_mixer_elem_t *elem ;
std::string VIcon = "";
// Setup variables used by both fucntions
char *device = "default";
/* char *selem_name = std::getenv("scontrol"); */
char *selem_name = "Master";
// Get $scontrol variable set in .bashrc
int C = -1;

static void error_close_exit(const char *errmsg, int err, snd_mixer_t *h_mixer) {
    if (err == 0)
		fprintf(stderr, "%s", errmsg);
    else
        fprintf(stderr, errmsg, snd_strerror(err));
    std::vector<std::string> Output;
    Output.push_back("");

    k::WriteFileLines(Output, VolumeOutputFile);
	if (h_mixer != NULL)
		snd_mixer_close(h_mixer);
	exit(EXIT_FAILURE);
}

int GetVolumeLevel(char *device, char *selem_name) {
    long vol;
	if ((err = snd_mixer_open(&h_mixer, 1)) < 0)
    	error_close_exit("Mixer open error: %s\n", err, NULL);
    
    if ((err = snd_mixer_attach(h_mixer, device)) < 0)
    	error_close_exit("Mixer attach error: %s\n", err, h_mixer);
    
    if ((err = snd_mixer_selem_register(h_mixer, NULL, NULL)) < 0)
    	error_close_exit("Mixer simple element register error: %s\n", err, h_mixer);
    
    if ((err = snd_mixer_load(h_mixer)) < 0)
    	error_close_exit("Mixer load error: %s\n", err, h_mixer);
    
    snd_mixer_selem_id_alloca(&sid);
    snd_mixer_selem_id_set_index(sid, 0);
    snd_mixer_selem_id_set_name(sid, selem_name);
    
    if ((elem = snd_mixer_find_selem(h_mixer, sid)) == NULL)
    	error_close_exit("Cannot find simple element\n", 0, h_mixer);
    
    snd_mixer_selem_get_playback_volume(elem, CHANNEL, &vol);
    
	snd_mixer_close(h_mixer);

    vol = round((float)vol * (1/VolScaler));

    return vol;
}

void SetVolumeLevel(char *device, char *selem_name, long vol) {
	if ((err = snd_mixer_open(&h_mixer, 1)) < 0)
		error_close_exit("Mixer open error: %s\n", err, NULL);

	if ((err = snd_mixer_attach(h_mixer, device)) < 0)
		error_close_exit("Mixer attach error: %s\n", err, h_mixer);

	if ((err = snd_mixer_selem_register(h_mixer, NULL, NULL)) < 0)
		error_close_exit("Mixer simple element register error: %s\n", err, h_mixer);

	if ((err = snd_mixer_load(h_mixer)) < 0)
		error_close_exit("Mixer load error: %s\n", err, h_mixer);

	snd_mixer_selem_id_alloca(&sid);
	snd_mixer_selem_id_set_index(sid, 0);
	snd_mixer_selem_id_set_name(sid, selem_name);

	if ((elem = snd_mixer_find_selem(h_mixer, sid)) == NULL)
		error_close_exit("Cannot find simple element\n", 0, h_mixer);

	if ((err = snd_mixer_selem_set_playback_volume_all(elem, vol)) < 0)
		error_close_exit("Volume set error: %s\n", err, h_mixer);

	snd_mixer_close(h_mixer);
}

int Volume() {
    int Level = GetVolumeLevel(device, selem_name);
    /* if(selem_name == "Master") { */
    /*     k::map( */
    /* } */
    if(Level >= VHigh) {
        VIcon = IVolHigh;
    } else if(Level >= VMid) {
        VIcon = IVolMid;
    } else if(Level >= VLow) {
        VIcon = IVolLow;
    } else if(Level == VMute) {
        VIcon = IVolMute;
    } else {
        std::cout << "Vol parsing error: -" << Level << "-" << std::endl;
    }
    
    std::vector<std::string> Output;
    Output.push_back(R"($(printf ")" + VCol[0] + VIcon + R"( %s" ")" + VCol[1] + std::to_string(Level) + R"(%"))");

     k::WriteFileLines(Output, VolumeOutputFile);

#ifdef VolMCOUT
    VPrint(Output);
#endif
	return 0;
	}

int main(int argc, char** argv) {
    FILE *file;
    if ((file = fopen("a.txt", "r"))) {
      fclose(file);
        if(k::ReadFileLines(BluetoothDataFile).at(0) == "1")
            selem_name = "Master";
    }
    // If no args are given get volume
	if (argc < 2) {
        Volume();
        return 0;
    }

    // If args are given assign to var after check
	float vol = atol(argv[1]);
    // Scale volume to comply with full volume level
    vol = round(vol * VolScaler);
    
    // Set volume to given level
    SetVolumeLevel(device, selem_name, vol);

	return 0;
}

