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
            break;
        case RENDER_STEREO:
            break;
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
            add_line_to_edge();
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

void add_line_to_edge() {
        edge = add_columns( edge, 2 );       
        edge.mat[edge.width - 2][0] = strtod(args[1], NULL);
        edge.mat[edge.width - 2][1] = strtod(args[2], NULL);
        edge.mat[edge.width - 2][2] = strtod(args[3], NULL);
        edge.mat[edge.width - 2][3] = 1;
        edge.mat[edge.width - 1][0] = strtod(args[4], NULL);
        edge.mat[edge.width - 1][1] = strtod(args[5], NULL);
        edge.mat[edge.width - 1][2] = strtod(args[6], NULL);
        edge.mat[edge.width - 1][3] = 1;
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

void convert_from_screen() {
    //translation_matrix
    //
    double ts[3];
    ts[0] = width / (sxr - sxl);
    ts[1] = height / (syr - syl);
    ts[2] = 1;
    int i;
    printf("Pre trans...\n");
    print_matrix( edge );
    matrix rot = rotation_matrix_y( -M_PI/ 2);
    edge = multiply_matrix( rot, edge );
    //rot = rotation_matrix_z( M_PI/ 4);
    //edge = multiply_matrix( rot, edge );

    for( i = 4; i < edge.width; i++ ) {
        edge.mat[i][0] = (width)  * (edge.mat[i][0] - sxl) / (sxr - sxl);
        edge.mat[i][1] = height - (height) * (edge.mat[i][1] - syl) / (syr - syl);
    }
    print_matrix( edge );
        matrix scaler = scale_matrix( ts );

    /*matrix mirror = init_identity( 4 );
    set_element( mirror, -1, 1, 1 );
    set_element( mirror, height, 2, 1 );
    //matrix transformist = multiply_matrix( mirror, multiply_matrix(scaler, translator ));
    matrix temp = multiply_matrix( transformist, edge );
    print_matrix( temp );
    delete_matrix( edge );
    edge = temp;
    edge = multiply_matrix( scaler, edge );
    matrix rotator = rotation_matrix_y( M_PI / 4 );
    edge = multiply_matrix( rotator, edge );
    rotator = rotation_matrix_z( 0 );
    edge = multiply_matrix( rotator, edge );
    //edge = multiply_matrix( mirror, edge );
    */
    //edge = multiply_matrix( scaler, edge );
    //delete_matrix( translator );
    delete_matrix( scaler );
    //delete_matrix( mirror );
}





