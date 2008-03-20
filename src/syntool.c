#include "syntool.h"

void
set_touchpad(SynapticsSHM *synshm, double val)
{
    struct Parameter* par = &params[0];
    
    *(Bool*)((char*)synshm + par->offset) = (Bool)rint(val);
}

int
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

SynapticsSHM* 
syn_init() 
{
	SynapticsSHM *synshm;
    int shmid;


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
    return synshm;
}

int
get_matrixcode(SynapticsSHM *cur, int xmax, int ymax, int zmin)
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
