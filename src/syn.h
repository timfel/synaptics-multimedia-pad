#ifndef SYN_H
#define SYN_H

#include "common.h"

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

void syn_set_touchpad(SynapticsSHM *synshm, double val);
int syn_is_equal(SynapticsSHM *s1, SynapticsSHM *s2);
int syn_get_matrixcode(SynapticsSHM *cur, Config* std);
SynapticsSHM* syn_init();


#endif
