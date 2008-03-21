#ifndef ONSCR_H
#define ONSCR_H


#include "common.h"

#ifdef XOSD
	// osd stuff
	#include <xosd.h>
	#include <locale.h>
	#include <X11/Xlib.h>
	#include <unistd.h>
	#include <fcntl.h>
	#include <sys/ioctl.h>

	// the volume osd color
	#define OSD_VCOLOR "red"
	// the brightness osd color
	#define OSD_BCOLOR "blue"
	// the osd font... change the 7th position for size
	// or use xfontsel to explore the options
	#define OSD_FONT "-*-*-*-*-*-*-20-*-*-*-*-*-*-*"
	// the length of time the osd is shown
	#define OSD_TIME 3

	// volume scaling factor for OSD
	#define OSD_VSCALE 250

	xosd* onscr_init();
	void onscr_mmm_on(xosd* disp_obj);
	void onscr_mmm_off(xosd* disp_obj);
	void onscr_volume(xosd* disp_obj, int value, const char* mixer);
	void onscr_action(xosd* disp_obj, const char* value);
#endif

#endif
