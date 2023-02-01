#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>
#include "../include/dwmbar-k.h" 

static void error_close_exit(const char *errmsg, int err, snd_mixer_t *h_mixer) {
	if (err == 0)
		fprintf(stderr, errmsg);
	else
		fprintf(stderr, errmsg, snd_strerror(err));
	if (h_mixer != NULL)
		snd_mixer_close(h_mixer);
	exit(EXIT_FAILURE);
}

int main(int argc, char** argv) {
	int err;
	long vol;
	snd_mixer_t *h_mixer;
	snd_mixer_selem_id_t *sid;
	snd_mixer_elem_t *elem ;
	char *device;
	char *selem_name;

	if (argc < 2) {
		printf("Usage:\n\t%s device control volume\n", argv[0]);
		return 0;
	}

	device = "default";
	selem_name = std::getenv("scontrol");
	vol = atol(argv[1]);

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
	return 0;
}

