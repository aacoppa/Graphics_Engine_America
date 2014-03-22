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
    /*while( args[i] ) {
        printf("%d : %s\n", i, args[i]);
        i++;
    }*/
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
            /*matrix temp = multiply_matrix( transformer, edge );
            delete_matrix( edge );
            edge = temp;*/
            convert_from_screen();
            int cols[3];
            cols[0] = 255;
            cols[1] = 255;
            cols[2] = 255;
            draw_triangles(cols);
            break;
            }
        case RENDER_CYCLOPS:
            {
            /*matrix temp = multiply_matrix( transformer, edge );
            delete_matrix( edge );
            edge = temp;*/
            double ex = strtod(args[1], NULL);
            double ey = strtod(args[2], NULL);
            double ez = strtod(args[3], NULL);
            eye.x = ex;
            eye.y = ey;
            eye.z = ez;
            //assert( ex && ey && ez );
            render_to_eye( ex, ey, ez );
            int cols[3];
            cols[0] = 255;
            cols[1] = 255;
            cols[2] = 255;
            draw_triangles(cols);
            break;
            }
        case SPHERE:
            {
            double r = strtod(args[1], NULL);
            double x = strtod(args[2], NULL);
            double y = strtod(args[3], NULL);
            double z = strtod(args[4], NULL);
            struct point ** points = draw_sphere(x, y, z, r);
            draw_triangles_in_sphere( points );
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
            matrix temp = copy_matrix( edge );

            int cols[3];
            cols[0] = 255;
            cols[1] = 0;
            cols[2] = 0;
            render_to_eye( ex1, ey1, ez1 );
            draw_triangles( cols );
            edge = temp;
            cols[0] = 0;
            cols[1] = 255;
            cols[2] = 255;
            render_to_eye( ex2, ey2, ez2 );
            draw_triangles( cols );
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
                theta = convert_to_radians( theta );
                new_transformer = rotation_matrix_x( theta );
                goto multiply_into_transformation;
            }
        case ROTATE_Y:
            {
                float theta = strtod( args[1], NULL );
                theta = convert_to_radians( theta );
                new_transformer = rotation_matrix_y( theta );
                goto multiply_into_transformation;
            }
        case ROTATE_Z:
            {
                float theta = strtod( args[1], NULL );
                theta = convert_to_radians( theta );
                new_transformer = rotation_matrix_z( theta );
                goto multiply_into_transformation;
            }
        case IDENTITY:
            delete_matrix( transformer );
            transformer = init_identity( 4 );
            break;
        case TRANSFORM:
            {
            matrix temp = multiply_matrix( transformer, edge );
            delete_matrix( edge );
            edge = temp;
            }
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
void add_triangle_to_edge( double x1, double y1, double z1,
                           double x2, double y2, double z2,
                           double x3, double y3, double z3 ) {
        //Order points in a counter clockwise order
        struct point p1, p2, p3;
        double min = x1 < x2 ? x1 : x2;
        double miny, maxy;
        min = min < x3 ? min : x3;
        if( min == x1 ) {
                p1.x = x1;
                p1.y = y1;
                p1.z = z1;
                miny = y2 < y3 ? y2 : y3;
                maxy = y2 < y3 ? y3 : y2;
        } else if( min == x2) {
                p1.x = x2;
                p1.y = y2;
                p1.z = z2;
                miny = y1 < y3 ? y1 : y3;
                maxy = y1 < y3 ? y3 : y1;
        } else {
                p1.x = x3;
                p1.y = y3;
                p1.z = z3;
                miny = y1 < y2 ? y1 : y2;
                maxy = y1 < y2 ? y2 : y1;
        }
        if(miny == y1) {
                p2.x = x1;
                p2.y = y1;
                p2.z = z1;
        } else if(miny == y2) {
                p2.x = x2;
                p2.y = y2;
                p2.z = z2;
        } else {
                p2.x = x3;
                p2.y = y3;
                p2.z = z3;
        }
        if(maxy == y1) {
                p3.x = x1;
                p3.y = y1;
                p3.z = z1;
        } else if(maxy == y2) {
                p3.x = x2;
                p3.y = y2;
                p3.z = z2;
        } else {
                p3.x = x3;
                p3.y = y3;
                p3.z = z3;
        }
        edge = add_columns( edge, 3 );       
        edge.mat[edge.width - 3][0] = p1.x;
        edge.mat[edge.width - 3][1] = p1.y;
        edge.mat[edge.width - 3][2] = p1.z;
        edge.mat[edge.width - 3][3] = 1;
        edge.mat[edge.width - 2][0] = p2.x;
        edge.mat[edge.width - 2][1] = p2.y;
        edge.mat[edge.width - 2][2] = p2.z;
        edge.mat[edge.width - 2][3] = 1;
        edge.mat[edge.width - 1][0] = p3.x;
        edge.mat[edge.width - 1][1] = p3.y;
        edge.mat[edge.width - 1][2] = p3.z;
        edge.mat[edge.width - 1][3] = 1;
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
void draw_triangles(int cols []) {
    int startX = 6;
    //matrix move_to_zero = translation_matrix( (sxr
    struct point p1, p2, p3;
    p1.x = edge.mat[startX-2][0];
    p1.y = edge.mat[startX-2][1];
    p1.z = edge.mat[startX-2][2];
    p2.x = edge.mat[startX-1][0];
    p2.x = edge.mat[startX-1][1];
    p2.x = edge.mat[startX-1][2];
    p3.x = edge.mat[startX][0];
    p3.x = edge.mat[startX][1];
    p3.x = edge.mat[startX][2];

   // if( get_direction(p1, p2, p3) < 0 ) return; //Not facing us
    while( startX < edge.width ) {
        draw_line( edge.mat[startX - 2][0], edge.mat[startX - 2][1],
                edge.mat[startX - 1][0], edge.mat[startX - 1][1], cols);
        draw_line( edge.mat[startX - 2][0], edge.mat[startX - 2][1],
                edge.mat[startX][0], edge.mat[startX][1], cols);
        draw_line( edge.mat[startX-1][0], edge.mat[startX-1][1],
                edge.mat[startX][0], edge.mat[startX][1], cols);       
        startX += 3;
    }
}
void convert_from_screen() {
    int i;

    for( i = 4; i < edge.width; i++ ) {
        double before = edge.mat[i][0] - (sxl);
        edge.mat[i][0] = (width)  * (edge.mat[i][0] - sxl) / (sxr - sxl); //Alter the x

        before = edge.mat[i][1] - (syl);
        //edge.mat[i][1] = height - ( (height) * (edge.mat[i][1] - syl) / (syr - syl) );
        edge.mat[i][1] = height * (before / (syr - syl));
        edge.mat[i][1] = height - edge.mat[i][1];
    }
}

double convert_to_radians(double theta) {
    return M_PI / 180 * theta;
}
void draw_triangles_in_sphere( struct point ** points ) {
    int i, j;
    for( i = 1; i < N_POINTS; i++) {
        //I is travelling vertically...
        for( j = 1; j < N_POINTS; j++) {
                struct point p1 = points[i - 1][j];
                struct point p2 = points[i - 1][j - 1];
                struct point p3 = points[i][j];
                struct point p4 = points[i][j-1];
                add_triangle_to_edge(p1.x, p1.y, p1.z,
                                       p2.x, p2.y, p2.z,
                                       p3.x, p3.y, p3.z);
                add_triangle_to_edge(p4.x, p4.y, p4.z,
                                         p2.x, p2.y, p2.z,
                                         p3.x, p3.y, p3.z);
        }
    }
    for(i = 1; i < N_POINTS; i++) {
        struct point p1 = points[i][0];
        struct point p2 = points[i][N_POINTS-1];
        struct point p3 = points[i-1][0];
        struct point p4 = points[i-1][N_POINTS-1];
        add_triangle_to_edge(p1.x, p1.y, p1.z,
                p2.x, p2.y, p2.z,
                p3.x, p3.y, p3.z);

        add_triangle_to_edge(p4.x, p4.y, p4.z,
                p2.x, p2.y, p2.z,
                p3.x, p3.y, p3.z);
    }
    for(i = 1; i < N_POINTS; i++) {
        struct point p1 = points[N_POINTS][i];
        struct point p2 = points[N_POINTS-1][i];
        struct point p3 = points[N_POINTS][i-1];
        struct point p4 = points[N_POINTS-1][i-1];
        add_triangle_to_edge(p1.x, p1.y, p1.z,
                p2.x, p2.y, p2.z,
                p3.x, p3.y, p3.z);

        add_triangle_to_edge(p4.x, p4.y, p4.z,
                p2.x, p2.y, p2.z,
                p3.x, p3.y, p3.z);
    }

}























