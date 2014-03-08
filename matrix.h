/*
 *      MATRIX:
 *      Initializing and multiplying matrixes
 *      
 *      multiply_matrix( m, m ); //returns new matrix
 *      delete_matrix( m ); //frees the inner array
 *
 */
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

void fill_matrix(matrix, double);
matrix init_identity(int);
matrix init_matrix(int, int);
void scalar_multiply(double, matrix);
void set_element( matrix, double, int, int );
matrix multiply_matrix(matrix, matrix);
double col_x_row(double *, matrix, int);
void print_matrix(matrix);
matrix add_column( matrix );
matrix add_columns( matrix, int );
void delete_matrix( matrix );

#endif