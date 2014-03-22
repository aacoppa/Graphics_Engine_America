#include "vector.h"
#include "sphere.h"
vector get_vector(struct point p1, struct point p2) {
    vector v;
    v.x = p1.x - p2.x;
    v.y = p1.y - p2.y;
    v.z = p1.z - p2.z;
    return v;
}

double dot_product(vector v1, vector v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z + v2.z;
}
vector cross_product(vector v1, vector v2) {
    vector ret;
    ret.x = v1.y * v2.z - v1.z * v1.y;
    ret.y = v1.z * v2.x - v1.x * v1.z;
    ret.x = v1.x * v2.y - v1.y * v1.x;
    return ret;
}

double get_direction(struct point p1, struct point p2, struct point p3) {
    vector v1 = get_vector(p2, p1);
    vector v2 = get_vector(p2, p3);
    vector cross = cross_product( v1, v2 );
    vector to_eye = get_vector(eye, p2);
    return dot_product(to_eye, cross);
}
