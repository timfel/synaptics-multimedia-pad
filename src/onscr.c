#include "onscr.h"

#ifdef XOSD
xosd* onscr_init()
{
	xosd* disp_obj;
	setlocale(LC_ALL, "");
                
    disp_obj = xosd_create(2);
  	if (disp_obj == NULL)
    {
    	perror ("Could not create \"osd\"");
    }
    	
    xosd_set_timeout(disp_obj, 2);
    xosd_set_font (disp_obj, "-b&h-*-bold-r-*-*-34-*-*-*-*-*-*-*");
    xosd_set_shadow_offset (disp_obj, 4);
    xosd_set_colour (disp_obj, "green1");
    xosd_set_outline_offset(disp_obj, 2);
	xosd_set_outline_colour(disp_obj, "black");
	xosd_set_pos (disp_obj, XOSD_bottom);
	xosd_set_vertical_offset (disp_obj, 48);
	xosd_set_align (disp_obj, XOSD_center);
	
	return disp_obj;
}

void onscr_mmm_on(xosd* disp_obj)
{
	xosd_scroll(disp_obj, 2);
	xosd_display (disp_obj, 0, XOSD_string, "You have been R00ted");
}

void onscr_mmm_off(xosd* disp_obj)
{
	xosd_scroll(disp_obj, 2);
	xosd_display (disp_obj, 0, XOSD_string, "Multimedia disabled");
}

void onscr_volume(xosd* disp_obj, int value, const char* mixer)
{
	char* title;
	title = (char*)malloc(sizeof(char)*(8+strlen(mixer)));
	strcpy(title, "Volume ");
	strcat(title, mixer);
	
	xosd_scroll(disp_obj, 2);
	xosd_display (disp_obj, 0, XOSD_string, title);
	xosd_display (disp_obj, 1, XOSD_percentage, value);
}

void onscr_action(xosd* disp_obj, const char* value)
{
	xosd_scroll(disp_obj, 2);
	xosd_display(disp_obj, 0, XOSD_string, "Command");
	xosd_display (disp_obj, 1, XOSD_string, value);
}
#endif
