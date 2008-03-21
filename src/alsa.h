#ifndef ALSA_H
#define ALSA_H

#include "common.h"

#ifdef ALSA
	#include <alsa/asoundlib.h>

	int make_noise(const char* filename);
	void set_volume(int percentage);
#endif

#endif
