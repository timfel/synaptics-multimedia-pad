#ifndef GSTREAMER_H
#define GSTREAMER_H

#include "common.h"

#ifdef GSTREAMER
  #include <gst/gst.h>
  int gstreamer_play(char* filename);
#endif

#endif
