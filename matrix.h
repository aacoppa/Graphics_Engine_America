#ifndef __MATRIX_AC
#define __MATRIX_AC 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int width;
    int height;
    double ** mat;
} matrix;

matrix init_matrix(int, int);
void scalar_multiply(double, matrix);
matrix multiply_matrix(matrix, matrix);
double col_x_row(double *, matrix, int);
void print_matrix(matrix);

#endif
