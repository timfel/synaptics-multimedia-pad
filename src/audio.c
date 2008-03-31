#include "audio.h"

int make_noise(char* filename)
{
  pthread_setcancelstate (PTHREAD_CANCEL_ENABLE, NULL);
  pthread_setcanceltype (PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
  
  #ifdef ALSA
	return alsa_make_noise(filename);
  #endif

  #ifdef GSTREAMER
	return gstreamer_play(filename);
  #endif
  return 0;
}
