/*
 *   Copyright 2002-2005,2007 Peter Osterlund <petero2@telia.com>
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation; either version 2
 *   of the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#include "common.h"
#include "syn.h"
#include "conf.h"
#include "audio.h"
#include "onscr.h"

Config std;
int mmmode = 0;
char* actions[9] = {std.a1, std.a2, std.a3, std.a4, std.a5, std.a6, std.a7, std.a8, std.a9};
#ifdef XOSD
	// the onscreen display
	xosd* osd = '\0';
#endif

static int
run_action(int actioncode)
{
    int i;
    int count = 0;
    char cmd[127] = {'\0'};
    char percentage[4] = {'\0'};
    pthread_t run;
    
    if (actioncode != 0) {
        if ((actioncode > 4) && (actioncode < 100)) {
            for(i = 10; i < 40; i=i+10) {
                if ((actioncode / i) == 1) {
                    switch ((actioncode % i)) {
                    case 1:
                        strcpy(cmd, actions[count]);
                        break;
                    case 2:
                        strcpy(cmd, actions[count+1]);
                        break;
                    case 3:
                        strcpy(cmd, actions[count+2]);
                        break;
                    }
                }
                count += 3;
            }
            #ifdef XOSD
            	if (std.osd == 1)
            		onscr_action(osd, cmd);
            #endif 
        } else {
        	#ifdef XOSD
        		if (std.osd == 1)
        			onscr_volume(osd, actioncode-100, (const char*)std.alsamixer);
        	#endif
        	sprintf(cmd,"amixer sset \"%s\" \"%d%\"",std.alsamixer,actioncode-100);
        }
        
        pthread_create(&run, NULL, system, cmd);
		return pthread_detach(run);
    }
    return 0;
}

static void
monitor(SynapticsSHM *synshm, int delay)
{
    SynapticsSHM old;
    int action = 0;
    int start = 1;
    
    memset(&old, 0, sizeof(SynapticsSHM));
    
    while (1) {
    SynapticsSHM cur = *synshm;
    if (!syn_is_equal(&old, &cur)) {
	    fflush(stdout);
	    old = cur;
	    fflush(stdout);
	    if ((synshm->x >= std.triggerx) && (synshm->y <= std.triggery) && (synshm->z >= synshm->finger_high)) {
	        if (mmmode == 0) {
	            mmmode = 1;
	            start = 1;
	            syn_set_touchpad(synshm, 1);            
                #ifdef XOSD
                	if (std.osd == 1)
                		onscr_mmm_on(osd);
                #endif
                if (std.actsound == 1) {
                	make_noise(std.soundon);
                }
            } else {
	            mmmode = 0;
	            syn_set_touchpad(synshm, 0);
                #ifdef XOSD
                	if (std.osd == 1)
                		onscr_mmm_off(osd);
                #endif
                if (std.actsound == 1) {
               		make_noise(std.soundoff);
                }
            }
        }
        if ((mmmode == 1) && (start == 0)) {
            action = syn_get_matrixcode(&cur, &std);
            run_action(action);
        } else
        	start = 0;
	}
	usleep(delay * 1000);
    }
}

int
main()
{
	int delay = 100;
    SynapticsSHM *synshm;
 
    synshm = syn_init();
    
    std = conf_read(&std);
    
    #ifdef XOSD
   		if (std.osd == 1)
   			osd = onscr_init();
   	#endif
    
    /* Perform requested actions */
	monitor(synshm, delay);
	
    /* set_touchpad(synshm, 0); */
    #ifdef XOSD
    	if (std.osd == 1)
    		xosd_destroy (osd);
    #endif
    exit(0);
}
