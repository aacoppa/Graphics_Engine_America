#include "matrix.h"

matrix init_matrix(int width, int height) {
    matrix m;
    m.mat = malloc( sizeof(int *) * width );
    int i, j;
    for(i = 0; i < width; i++) {
        m.mat[i] = malloc(sizeof(int) * height);
        for( j = 0; j < height; j++) {
            m.mat[i][j] = 1; 
        }
    }
    m.width = width;
    m.height = height;
    return m;
}
void scalar_multiply(int S, matrix m) {
    int i, j;
    for(i = 0; i < m.height; i++) 
        for(j = 0; j < m.width; j++) 
            m.mat[i][j] *= S;
}
matrix multiply_matrix(matrix a, matrix b) {
    if( a.width != b.height ) {
        fprintf(stderr, "Error: Matrixes of [%d x %d], [%d x %d] cannot be multiplied\n", a.width, a.height, b.width, b.height);
        matrix m;
        return m; 
    }
    
    int ** ret = malloc( sizeof(int *) * b.width );
    int i;
    for(i = 0; i < b.width; i++) {
        ret[i] = malloc(sizeof(int) * a.height);
    }
    int ay, bx;
    ay = bx = 0;
    while( ay < a.height ) {
        while( bx < b.width ) {
            //Set current row
            ret[bx][ay] = col_x_row( b.mat[bx], a, ay );
            bx++;
        }
        ay++;
        bx = 0;
    }

    matrix m;
    m.width = b.width;
    m.height = a.height;
    m.mat = ret;
    return m;
}

int col_x_row( int * col, matrix a, int r_num ) {
    int i, ret;
    i = ret = 0;
    while( i < a.width ) {
        ret += (a.mat[i][r_num] * col[i]);
        i++;
    }
    return ret;
}

void print_matrix(matrix m) {
    int i, j;
    for(i = 0; i < m.height; i++) {
        for(j = 0; j < m.width; j++) {
            printf("%d\t", m.mat[j][i]);
        }
        printf("\n");
    }
}
