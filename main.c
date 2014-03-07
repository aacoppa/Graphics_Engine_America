#include "matrix.h"

int main() {
    matrix m1 = init_matrix(2, 4);
    matrix m2 = init_matrix(5, 5);
    multiply_matrix(m1, m2);
    matrix m3 = init_matrix(4, 2);
    print_matrix( m1 );
    printf("\n");
    print_matrix( m3 );
    printf("\n");
    matrix m4 = multiply_matrix(m1, m3 );
    matrix m4_iden = init_identity( 4 );
    print_matrix( multiply_matrix( m4, m4_iden ) );
}
