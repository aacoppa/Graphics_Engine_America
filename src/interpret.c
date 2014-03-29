#include "interpret.h"
int main(int argc, char ** argv) {
    /*fn = malloc(20);
    assert( argv[1] );
    strcpy(fn, argv[1]);
    init();
    int c;
    while( (c = handle_type()) ) {
        //Just run handle_type
    }*/
    init_screen(-2, -2, 2, 2, 500, 500);
    float theta = M_PI / 3;
    struct timeval start, end;
    long mtime, secs, usecs;    
    int i = 0;
    matrix edge = init_identity(4);
    draw_box(1, 1, 1, 1, 0, 0, &edge);
    //draw_sphere(-1, 0, 0, 1, &edge);
    SDL_Color s;
    s.r = 0;
    s.g = 0;
    s.b = 255;
    float total_secs = 0;
    float total_frames = 0;
    while( i < 1000 ) {
    gettimeofday(&start, NULL);
    theta = theta + M_PI / 30;
    clearScreen();
    matrix transformer = init_identity( 4 );
    multiply_matrix_onto_self(rotation_matrix_x(theta), &transformer);
    //multiply_matrix_onto_self(rotation_matrix_z(theta), &transformer);
    //multiply_matrix_onto_self(rotation_matrix_y(theta), &transformer);
    matrix to_render = multiply_matrix(transformer, edge);
    draw_to_screen(0, 0, 5, &to_render, *(Uint32 *)&s);
    gettimeofday(&end, NULL);
    secs  = end.tv_sec  - start.tv_sec;
    usecs = end.tv_usec - start.tv_usec;
    mtime = ((secs) * 1000 + usecs/1000.0) + 0.5;
    //printf("Elapsed time: %ld millisecs\n", mtime);
    double l = (double) mtime / 1000;
    //printf("Or %f seconds\n", l);
    total_frames++;
    total_secs += l;
    l = 1 / l;
    /*printf("Frames per sec: %f\n", l);
    //1 / framerate = 1000 * millseconds
    //ez = ez + 1;*/
    i++;
    }
    printf("Total frames: %f\n", total_frames);
    printf("Total time: %f\n", total_secs);
    printf("Average fps: %f\n", total_frames / total_secs);
    return 0;
}
void init() {
    fp = fopen(fn, "r");
    transformer = init_identity( 4 );
    interpret_renderer = init_identity( 4 );
}
int next_type() {
    char buffer[1001];
    fgets(buffer, 1000, fp);
    args = parse_split( buffer );
    /*int i = 0;
    while( args[i] ) {
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
    if( !strcmp( args[0], "clear-triangles") ) return CLEAR_EDGES;
    if( !strcmp( args[0], "sphere-t") ) return SPHERE;
    if( !strcmp( args[0], "box-t") ) return BOX;
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
            //draw_triangles(&edge);
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
            struct point eye;
            eye.x = ex;
            eye.y = ey;
            eye.z = ez;
            SDL_Color s;
            s.r = 0;
            s.g = 0;
            s.b = 0;
            draw_to_screen( ex, ey, ez, &interpret_renderer, *(Uint32 *)&s);
            break;
            }
        case BOX:
            {
            double width = strtod(args[1], NULL);
            double height = strtod(args[2], NULL);
            double depth = strtod(args[3], NULL);

            double x = strtod(args[4], NULL);
            double y = strtod(args[5], NULL);
            double z = strtod(args[6], NULL);
            draw_box(width, height, depth, x, y, z, &interpret_renderer);
            break;
            }
        case SPHERE:
            {
            double r = strtod(args[1], NULL);
            double x = strtod(args[2], NULL);
            double y = strtod(args[3], NULL);
            double z = strtod(args[4], NULL);

            draw_sphere(x, y, z, r, &interpret_renderer);
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
            SDL_Color s;
            s.r = 0;
            s.g = 255;
            s.b = 255;
            draw_to_screen( ex1, ey1, ez1, &interpret_renderer, *(Uint32 *)&s);
            s.r = 255;
            s.g = 0;
            s.b = 0;
            draw_to_screen( ex2, ey2, ez2, &interpret_renderer, *(Uint32 *)&s);
            break;
            }
        case SCREEN:
            sxl = strtod(args[1], NULL);
            syl = strtod(args[2], NULL);
            sxr = strtod(args[3], NULL);
            syr = strtod(args[4], NULL);
            break;
        case PIXELS:
            init_screen(sxl, syl, sxr, syr, atoi(args[1]), atoi(args[2]));

            break;
        case CLEAR_PIXELS:
            //clear_background();
            clearScreen();
            break;
        case CLEAR_EDGES:
            delete_matrix(interpret_renderer);
            interpret_renderer = init_identity( 4 );
            break;
        case NAME:
             //write_array( args[1] );
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
            matrix temp = multiply_matrix( transformer, interpret_renderer);
            delete_matrix(interpret_renderer);
            interpret_renderer = temp;
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
double convert_to_radians(double theta) {
    return M_PI / 180 * theta;
}

