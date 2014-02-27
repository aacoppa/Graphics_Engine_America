#ifndef __MATRIX_AC
#define __MATRIX_AC 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int width;
    int height;
    int ** mat;
} matrix;

matrix init_matrix(int, int);
void scalar_multiply(int, matrix);
matrix multiply_matrix(matrix, matrix);
int col_x_row(int *, matrix, int);
void print_matrix(matrix);

#endif
