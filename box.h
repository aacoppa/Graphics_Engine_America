/* Rectangles
 */
#ifndef __CUBE_ACG
#define __CUBE_ACG 0
#include "sphere.h" //For struct point
#define N_DIVISIONS 6 
struct face * draw_cube(double, double, double,
                        double, double, double);
struct face {
    struct point ** points;
};

void free_faces(struct face *);
#endif
