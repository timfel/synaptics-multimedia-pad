#include "syn.h"

void
syn_set_touchpad(SynapticsSHM *synshm, double val)
{
    struct Parameter* par = &params[0];
    
    *(Bool*)((char*)synshm + par->offset) = (Bool)rint(val);
}

int
syn_is_equal(SynapticsSHM *s1, SynapticsSHM *s2)
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
syn_get_matrixcode(SynapticsSHM *cur, Config* std)
{
	int actioncode = 0;
    int buttonx, buttony;
    buttonx = std->xmax - (std->xmax / 8);  /* Take the sidescroll off the grid */
    buttony = cur->y;
    
    if ((cur->left == 1) || (cur->right == 1)) 
    {
        actioncode = 4;			/* If user clicks, drop out off mmmode */
        syn_set_touchpad(cur, 0);
    }
    
    if (cur->z > std->zmin) {
        if ((cur->x > 0) && (cur->x < buttonx/3))
            actioncode = 10;
        if ((cur->x > buttonx/3) && (cur->x < 2*buttonx/3))
            actioncode = 20;
        if ((cur->x > 2*buttonx/3) && (cur->x < buttonx))
            actioncode = 30;
        if ((cur->y > 0) && (cur->y < std->ymax/3))
            actioncode += 1;
        if ((cur->y > std->ymax/3) && (cur->y < 2*std->ymax/3))
            actioncode += 2;
        if ((cur->y > 2*std->ymax/3) && (cur->y < std->ymax))
            actioncode += 3;
    }
    
    if ((actioncode < 4) && (cur->y > std->triggery))
    {
    	buttony = std->ymax - cur->y + std->triggery; /* Map the value to scale properly */
    	actioncode = buttony * 100 / std->ymax + 100; /* W/G = p/100 ;) */
    }
    return actioncode;
}
