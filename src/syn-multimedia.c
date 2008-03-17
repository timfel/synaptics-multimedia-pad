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

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <stddef.h>
#include <math.h>

#include <X11/Xdefs.h>
#include "synaptics.h"

#define CR 13            /* Decimal code of Carriage Return char */
#define LF 10            /* Decimal code of Line Feed char */
#define EOF_MARKER 26    /* Decimal code of DOS end-of-file marker */

enum ParaType {
    PT_INT,
    PT_BOOL,
    PT_DOUBLE
};

struct Parameter {
    char *name;				    /* Name of parameter */
    int offset;				    /* Offset in shared memory area */
    enum ParaType type;			    /* Type of parameter */
    double min_val;			    /* Minimum allowed value */
    double max_val;			    /* Maximum allowed value */
};

#define DEFINE_PAR(name, memb, type, min_val, max_val) \
{ name, offsetof(SynapticsSHM, memb), (type), (min_val), (max_val) }

static struct Parameter params[] = {
    DEFINE_PAR("TouchpadOff",          touchpad_off,            PT_INT,    0, 2)
};

int xmax = 980;
int ymax = 750;
int zmin = 30;
int triggerx = 960;
int triggery = 80;
int mmmode = 0;
char a1[127] = {'\0'};
char a2[127] = {'\0'};
char a3[127] = {'\0'};
char a4[127] = {'\0'};
char a5[127] = {'\0'};
char a6[127] = {'\0'};
char a7[127] = {'\0'};
char a8[127] = {'\0'};
char a9[127] = {'\0'};
char* actions[9] = {a1, a2, a3, a4, a5, a6, a7, a8, a9};
char alsamixer[10] = {'\0'};

static void
set_touchpad(SynapticsSHM *synshm, double val)
{
    struct Parameter* par = &params[0];
    
    *(Bool*)((char*)synshm + par->offset) = (Bool)rint(val);
}

static int
is_equal(SynapticsSHM *s1, SynapticsSHM *s2)
{
    int i;

    if ((s1->x           != s2->x) ||
	(s1->y           != s2->y) ||
	(s1->left        != s2->left) ||
	(s1->right       != s2->right))
	return 0;

    for (i = 0; i < 8; i++)
	if (s1->multi[i] != s2->multi[i])
	    return 0;

    return 1;
}

static int
read_config()
{
    char configfile[40] = {'\0'};
    FILE* cfile;
    char buffer[10] = {'\0'};
    char c;
    char buffer2[127] = {'\0'};
    int i;
    
    strcpy(configfile, getenv("HOME"));
    strcat(configfile, "/.medion-multimedia.cfg");
    printf("Configfile: %s \n", configfile);
    
    cfile = fopen((const char*)configfile, "r");
    
    if (cfile != NULL) {
        printf("%4s %4s %5d %5d %5d %5d %5d %5d %5d %5d %5d %5s\n",
                "MaxX", "MaxY", 1,2,3,4,5,6,7,8,9, "Mixer");
        for (i = 0; i < 15; i++) {
            (void)fscanf(cfile, "%s %s", buffer, buffer2);
            c = fgetc(cfile);
            while (!(c == CR || c == LF || c == EOF)) {
                strncat(buffer2, &c, 1);
                c = fgetc(cfile);
            }
            
            if (strstr(buffer, "xmax") != NULL)
                xmax = atoi(buffer2);
            else if (strstr(buffer, "ymax") != NULL)
                ymax = atoi(buffer2);
            else if (strstr(buffer, "zmin") != NULL)
                zmin = atoi(buffer2);
            else if (strstr(buffer, "triggerx") != NULL)
                triggerx = atoi(buffer2);
            else if (strstr(buffer, "triggery") != NULL)
                triggery = atoi(buffer2);
            else if (strstr(buffer, "action") != NULL)
                strcpy(actions[atoi(&buffer[6])-1], buffer2);
            else if (strstr(buffer, "mixer") != NULL)
                strcpy(alsamixer, buffer2);
            else
                printf("Unmapped option in line %d. Check cfg!\n", i+1); 
        }
        printf("%3d %3d %5s %5s %5s %5s %5s %5s %5s %5s %5s %s\n",
                    xmax, ymax, a1, a2, a3, a4, a5, a6, a7, a8, a9,
                    alsamixer);
        fflush(stdout);
        
        return 0;
    } else 
        return -1;
}

static int
run_action(int actioncode)
{
    int i;
    int count = 0;
    char cmd[127] = {'\0'};
    
    if (actioncode != 0) {
        if (actioncode > 4) {
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
        } else {
            strcpy(cmd, "amixer sset \"");
            strcat(cmd, alsamixer);
            switch (actioncode) {
            case 1:
                strcat(cmd, "\" \"100%\"");
                break;
            case 2:
                strcat(cmd, "\" \"50%\"");
                break;
            case 3:
                strcat(cmd, "\" \"10%\"");
                break;
            }
        }
    
        strcat(cmd, " &");
        if (cmd[1] != '&')
            return system(cmd);
    }
    return 0;
}

static int
get_actioncode(SynapticsSHM *cur)
{
    int actioncode = 0;
    int buttonx;
    buttonx = xmax - (xmax / 8);
    
    if ((cur->left == 1) || (cur->right == 1))
        actioncode = 4;
    
    if (cur->z > zmin) {            
        if ((cur->x > 0) && (cur->x < buttonx/3))
            actioncode = 10;
        if ((cur->x > buttonx/3) && (cur->x < 2*buttonx/3))
            actioncode = 20;
        if ((cur->x > 2*buttonx/3) && (cur->x < buttonx))
            actioncode = 30;
        if ((cur->y > 0) && (cur->y < ymax/3))
            actioncode += 1;
        if ((cur->y > ymax/3) && (cur->y < 2*ymax/3))
            actioncode += 2;
        if ((cur->y > 2*ymax/3) && (cur->y < ymax))
            actioncode += 3;
    }
    return actioncode;
}

static void
monitor(SynapticsSHM *synshm, int delay)
{
    SynapticsSHM old;
    int action = 0;
    
    memset(&old, 0, sizeof(SynapticsSHM));

    while (1) {
    SynapticsSHM cur = *synshm;
	if (!is_equal(&old, &cur)) {
	    fflush(stdout);
	    old = cur;
	    if ((synshm->x >= triggerx) && (synshm->y <= triggery) && (synshm->z >= zmin)) {
	        if (mmmode == 0) {
	            mmmode = 1;
	            set_touchpad(synshm, 1);
            } else {
	            mmmode = 0;
	            set_touchpad(synshm, 0);
            }
        }
        if (mmmode == 1) {
            action = get_actioncode(&cur);
            run_action(action);
        }
	}
	usleep(delay * 1000);
    }
}

int
main()
{
    SynapticsSHM *synshm;
    int shmid;
    int delay = 100;

    if ((shmid = shmget(SHM_SYNAPTICS, sizeof(SynapticsSHM), 0)) == -1) {
	if ((shmid = shmget(SHM_SYNAPTICS, 0, 0)) == -1) {
	    fprintf(stderr, "Can't access shared memory area. SHMConfig disabled?\n");
	    exit(1);
	} else {
	    fprintf(stderr, "Incorrect size of shared memory area. Incompatible driver version?\n");
	    exit(1);
	}
    }
    if ((synshm = (SynapticsSHM*) shmat(shmid, NULL, 0)) == NULL) {
	perror("shmat");
	exit(1);
    }
    
    /* set_touchpad(synshm, 1); */
    read_config();

    /* Perform requested actions */
	monitor(synshm, delay);
	
    /* set_touchpad(synshm, 0); */
    exit(0);
}
