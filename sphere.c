#include "sphere.h"

struct point ** draw_sphere( double x, double y, double z, double r ) {
    double phi, theta;
    struct point ** points = malloc( (N_POINTS + 1) * sizeof( struct point **) );
    int i, j;
    i = 0;
    for( phi = 0; phi <= M_PI + .01; phi += (M_PI / N_POINTS)) {
        points[i] = malloc(sizeof( struct point ) * (N_POINTS));
        j = 0;
        for( theta = 0; theta <= 2 * M_PI; theta += 2 * M_PI / N_POINTS) {
            //points[i][j] = malloc(sizeof( struct point ));
            //Make the points going around and connect them
            points[i][j].x = x + r * sin( phi ) * cos( theta );
            points[i][j].y = y + r * sin( phi ) * sin( theta );
            points[i][j].z = z + r * cos( phi );
            //Connect the point to the last one
            j++;
        }
        i++;
    }
    return points;
}

void free_points(struct point ** points) {
    int i;
    i = 0;
    while(i <= N_POINTS) {
        free(points[i]);
        i++;
    }
    free(points);
}
/*void add_line_to_edge( double a, double b, double c, double d, double e, double f ) {
    printf("Making one line:\n%f %f %f\n%f %f %f\n", a, b, c, d, e, f);
}*/
void print_point( struct point p ) {
    printf("Printing point: (x, y, z) (%f, %f, %f)\n",
            p.x, p.y, p.z);
}
