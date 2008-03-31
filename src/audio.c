#include "audio.h"

int make_noise(char* filename)
{
  int value = 0;
  #ifdef ALSA
  	pthread_t p;
	pthread_create(&p, NULL, alsa_make_noise, filename);
	value = pthread_detach(p);
  #endif

  #ifdef GSTREAMER
    pthread_t p;
	pthread_create(&p, NULL, gstreamer_play, filename);
	value = pthread_detach(p);
  #endif
  return value;
}
