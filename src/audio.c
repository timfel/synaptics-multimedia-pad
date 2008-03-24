#include "audio.h"

int make_noise(char* filename)
{
  #ifdef ALSA
	return alsa_make_noise(filename);
  #endif

  #ifdef GSTREAMER
	return gstreamer_play(filename);
  #endif
}
