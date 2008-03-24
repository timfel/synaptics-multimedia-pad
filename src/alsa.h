#ifndef ALSA_H
#define ALSA_H

#include "common.h"

#ifdef ALSA
	#include <alsa/asoundlib.h>

	int alsa_make_noise(char* filename);
	void alsa_set_volume(int percentage);
#endif

#endif
