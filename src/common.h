#ifndef COMMON_H
#define COMMON_H

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
#include <pthread.h>

#include <X11/Xdefs.h>
#include "../config.h"

#if SYN_VERSION==0145
	#include "synaptics_0_14_5.h"
#endif
#if SYN_VERSION==0146
	#include "synaptics_0_14_6.h"
#endif


#define CR 13            /* Decimal code of Carriage Return char */
#define LF 10            /* Decimal code of Line Feed char */
#define EOF_MARKER 26    /* Decimal code of DOS end-of-file marker */

typedef struct {
    int xmax;
	int ymax;
	int zmin;
	int triggerx;
	int triggery;
	int osd;
	int actsound;
	char soundon[239];
	char soundoff[239];
	char corner[3];
	char a1[127];
	char a2[127];
	char a3[127];
	char a4[127];
	char a5[127];
	char a6[127];
	char a7[127];
	char a8[127];
	char a9[127];
	char alsamixer[10];
} Config;

#endif
