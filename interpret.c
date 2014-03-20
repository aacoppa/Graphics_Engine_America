#include "interpret.h"
int main(int argc, char ** argv) {
    fn = malloc(20);
    assert( argv[1] );
    strcpy(fn, argv[1]);
    init();
    int c;
    while( (c = handle_type()) ) {
        //Just run handle_type
    }
}
void init() {
    fp = fopen(fn, "r");
    transformer = init_identity( 4 );
    edge = init_identity( 4 );
}
int next_type() {
    char buffer[1001];
    fgets(buffer, 1000, fp);
    args = parse_split( buffer );
    int i = 0;
    while( args[i] ) {
        printf("%d : %s\n", i, args[i]);
        i++;
    }
    if( fp == NULL || !args[0]) return ERROR;
    if( args[0][0] == '#' ) return COMMENT;
    if( !strcmp( args[0], "line") ) return LINE;
    if( !strcmp( args[0], "move") ) return TRANSLATE;
    if( !strcmp( args[0], "scale") ) return SCALE;
    if( !strcmp( args[0], "rotate-x") ) return ROTATE_X;
    if( !strcmp( args[0], "rotate-y") ) return ROTATE_Y;
    if( !strcmp( args[0], "rotate-z") ) return ROTATE_Z;
    if( !strcmp( args[0], "transform") ) return TRANSFORM;
    if( !strcmp( args[0], "screen") ) return SCREEN;
    if( !strcmp( args[0], "pixels") ) return PIXELS;
    if( !strcmp( args[0], "render-parallel") ) return RENDER_PARALLEL;
    if( !strcmp( args[0], "render-perspective-cyclops") ) return RENDER_CYCLOPS;
    if( !strcmp( args[0], "render-perspective-stereo") ) return RENDER_STEREO;
    if( !strcmp( args[0], "file") ) return NAME;
    if( !strcmp( args[0], "identity") ) return IDENTITY;
    if( !strcmp( args[0], "clear-pixels") ) return CLEAR_PIXELS;
    if( !strcmp( args[0], "clear-edges") ) return CLEAR_EDGES;
    if( !strcmp( args[0], "sphere") ) return SPHERE;
    if( !strcmp( args[0], "end") ) return QUIT;
    return ERROR;
}
int handle_type() {
    int type = next_type(); //Figure out type of data
    matrix new_transformer;
    switch (type) {   
        case RENDER_PARALLEL:
            {
            matrix temp = multiply_matrix( transformer, edge );
            delete_matrix( edge );
            edge = temp;
            convert_from_screen();
            draw_lines();
            break;
            }
        case RENDER_CYCLOPS:
            {
            matrix temp = multiply_matrix( transformer, edge );
            delete_matrix( edge );
            edge = temp;
            double ex = strtod(args[1], NULL);
            double ey = strtod(args[2], NULL);
            double ez = strtod(args[3], NULL);
            //assert( ex && ey && ez );
            render_to_eye( ex, ey, ez );
            draw_lines();
            break;
            }
        case SPHERE:
            {
            double r = strtod(args[1], NULL);
            double x = strtod(args[2], NULL);
            double y = strtod(args[3], NULL);
            double z = strtod(args[4], NULL);
            struct point ** points = draw_sphere(x, y, z, r);
            int i, j;
            double phi, theta;
            phi = 0;
            theta = 0;
            for(i = 0; i <= N_POINTS; i++) {
                for(j = 0; j < N_POINTS; j++) {
                    if( i != 0 ) {
                        add_line_to_edge( points[i][j].x, points[i][j].y, points[i][j].z,
                                points[i-1][j].x, points[i-1][j].y, points[i-1][j].z);
                    }
                    if( j != 0 ) {
                        add_line_to_edge( points[i][j].x, points[i][j].y, points[i][j].z,
                                points[i][j-1].x, points[i][j-1].y, points[i][j-1].z);
                    }
                    theta += (M_PI * 2/ N_POINTS);
                }
                phi += (M_PI / N_POINTS);
            }
            i = 0;
            for(i = 0; i < N_POINTS; i++) {
                        add_line_to_edge( points[i][0].x, points[i][0].y, points[i][0].z,
                                          points[i][N_POINTS-1].x, points[i][N_POINTS-1].y, points[i][N_POINTS-1].z);
            }

            break;
            }
        case RENDER_STEREO:
            {
            double ex1 = strtod(args[1], NULL);
            double ey1 = strtod(args[2], NULL);
            double ez1 = strtod(args[3], NULL);
            double ex2 = strtod(args[4], NULL);
            double ey2 = strtod(args[5], NULL);
            double ez2 = strtod(args[6], NULL);
            //assert( ex1 && ey1 && ez1 && ex2 && ey2 && ez2 );
            matrix temp = copy_matrix( edge );

            int cols[3];
            cols[0] = 255;
            cols[1] = 0;
            cols[2] = 0;
            render_to_eye( ex1, ey1, ez1 );
            draw_colored_lines( cols );
            edge = temp;
            cols[0] = 0;
            cols[1] = 0;
            cols[2] = 255;
            render_to_eye( ex2, ey2, ez2 );
            draw_colored_lines( cols );
            break;
            }
        case SCREEN:
            sxl = strtod(args[1], NULL);
            assert(sxl);
            syl = strtod(args[2], NULL);
            assert(syl);
            sxr = strtod(args[3], NULL);
            assert(sxr);
            syr = strtod(args[4], NULL);
            assert(syr);
            break;
        case PIXELS:
            init_background( atoi(args[1]), atoi(args[2]) );
            break;
        case CLEAR_PIXELS:
            clear_background();
            break;
        case CLEAR_EDGES:
            delete_matrix( edge );
            edge = init_identity( 4 );
            break;
        case NAME:
             write_array( args[1] );
             break;
        case LINE:
            add_line_to_edge(atof(args[1]), atof(args[2]), atof(args[3]), atof(args[4]), atof(args[5]), atof(args[6]));
            break;
        case QUIT:
            return 0;
        case ERROR:
            fprintf(stderr, "Error interpretting file\n");
            return 0;
        case COMMENT:
            //Skip to next reading
            return 1;
        case TRANSLATE:
            {
                int i = 1;
                double data [MAX_WORDS];
                while( args[i] ) {
                    data[i-1] = strtod(args[i], NULL);
                    i++;
                }
                new_transformer = translation_matrix( data );
                goto multiply_into_transformation;
            }
        case SCALE:
            {
                int i = 1;
                double data [MAX_WORDS];
                while( args[i] ) {
                    data[i-1] = strtod(args[i], NULL);
                    i++;
                }
                new_transformer = scale_matrix( data );
                goto multiply_into_transformation;
            }
        case ROTATE_X:
            {
                float theta = strtod( args[1], NULL );
                assert( theta );
                new_transformer = rotation_matrix_x( theta );
                goto multiply_into_transformation;
            }
        case ROTATE_Y:
            {
                float theta = strtod( args[1], NULL );
                assert( theta );
                new_transformer = rotation_matrix_y( theta );
                goto multiply_into_transformation;
            }
        case ROTATE_Z:
            {
                float theta = strtod( args[1], NULL );
                assert( theta );
                new_transformer = rotation_matrix_z( theta );
                goto multiply_into_transformation;
            }
        case IDENTITY:
            delete_matrix( transformer );
            transformer = init_identity( 4 );
            break;
        default:
            break;
    }
    return 1;

    multiply_into_transformation:
        {
            matrix t = multiply_matrix( new_transformer, transformer );
            delete_matrix( transformer );
            delete_matrix( new_transformer );
            transformer = t;
            return 1;
        }
}

void add_line_to_edge(double x1, double y1, double z1, double x2, double y2, double z2) {
        edge = add_columns( edge, 2 );       
        edge.mat[edge.width - 2][0] = x1;
        edge.mat[edge.width - 2][1] = y1;
        edge.mat[edge.width - 2][2] = z1;
        edge.mat[edge.width - 2][3] = 1;
        edge.mat[edge.width - 1][0] = x2;
        edge.mat[edge.width - 1][1] = y2;
        edge.mat[edge.width - 1][2] = z2;
        edge.mat[edge.width - 1][3] = 1;
        //printf("%f %f %f %f %f %f\n", edge.mat[edge.width - 2][0], edge.mat[edge.width - 2][1],edge.mat[edge.width - 2][2],edge.mat[edge.width - 1][0],edge.mat[edge.width - 1][1],edge.mat[edge.width - 1][2]);
}

void render_to_eye( double ex, double ey, double ez) {
    int i;
    for( i = 4; i < edge.width; i++ ) {
        //edge.mat[i][0] = -ez * ( (edge.mat[i][0] - ex) / edge.mat[i][2] - ez ) + ex;
        //edge.mat[i][1] = (edge.mat[i][2] - ez) / ( -ez ) * ( edge.mat[i][1] - ey ) + ey;  
        edge.mat[i][0] = (-ez / (edge.mat[i][2] - ez) ) * ( edge.mat[i][0] - ex) + ex;
        edge.mat[i][1] = (-ez / (edge.mat[i][2] - ez) ) * ( edge.mat[i][1] - ey) + ey;
        edge.mat[i][0] = (width)  * (edge.mat[i][0] - sxl) / (sxr - sxl);
        edge.mat[i][1] = height - ( (height) * (edge.mat[i][1] - syl) / (syr - syl) );
    }

}
void draw_lines() {
        int startX = 5;
        int cols[3];
        cols[0] = 255;
        cols[1] = 255;
        cols[2] = 255;
        //matrix move_to_zero = translation_matrix( (sxr
        while( startX < edge.width ) {
            draw_line( edge.mat[startX - 1][0], edge.mat[startX - 1][1], edge.mat[startX][0], edge.mat[startX][1], cols);
            startX += 2;
        }
}
void draw_colored_lines( int cols[] ) {
    int startX = 5;
    while( startX < edge.width ) {
            draw_line( edge.mat[startX - 1][0], edge.mat[startX - 1][1], edge.mat[startX][0], edge.mat[startX][1], cols);
            startX += 2;
        }

}
void convert_from_screen() {
    //translation_matrix
    //
    /*double ts[3];
    ts[0] = width / (sxr - sxl);
    ts[1] = height / (syr - syl);
    ts[2] = 1; */
    /*print_matrix( edge );
    matrix rot = rotation_matrix_y( -M_PI/ 2);
    matrix temp = multiply_matrix( rot, edge );
    */
    //rot = rotation_matrix_z( M_PI/ 4);
    //edge = multiply_matrix( rot, edge );
    int i;

    for( i = 4; i < edge.width; i++ ) {
        //printf( "Before %f, %f\n", edge.mat[i][0], edge.mat[i][1] );
        edge.mat[i][0] = (width)  * (edge.mat[i][0] - sxl) / (sxr - sxl);
        edge.mat[i][1] = height - ( (height) * (edge.mat[i][1] - syl) / (syr - syl) );
        //printf( "After %f, %f\n", edge.mat[i][0], edge.mat[i][1] );
    }
}





