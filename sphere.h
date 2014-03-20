#ifndef __TRANS_SPH_AJS
#define __TRANS_SPH_AJS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "line.h"
#define N_POINTS 20 

struct point {
    double x, y, z;
};

//void add_line_to_edge( double, double, double, double, double, double );
struct point ** draw_sphere( double, double, double, double );
#endif
