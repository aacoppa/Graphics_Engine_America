#ifndef __VECTOR_ACG
#define __VECTOR_ACG 1
typedef struct {
    double x;
    double y;
    double z;
} vector;

struct point eye;
double get_direction(struct point, struct point, struct point);
vector get_vector(struct point, struct point);
double dot_product(vector, vector);
vector cross_product(vector, vector);
#endif
