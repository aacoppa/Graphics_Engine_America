#include "renderer.h"

void init_screen(double SXL, double SYL, double SXR, double SYR, int w, int h) {
    sxr = SXR;
    syr = SYR;
    sxl = SXL;
    syl = SYL;
    configureScreen(w, h);
    width = w;
    height = h;
}
void draw_to_screen(double ex, double ey, double ez, matrix * to_render, Uint32 color) {
    clearScreen();

    render_to_eye(ex, ey, ez, to_render);
    struct point eye;
    eye.x = ex;
    eye.y = ey;
    eye.z = ez;
    draw_triangles(to_render, eye, color);
    renderScreen();
}
void add_triangle_to_render( double x1, double y1, double z1,
                           double x2, double y2, double z2,
                           double x3, double y3, double z3, matrix * to_render) {
        //Order points in a counter clockwise order
    (*to_render) = add_columns((*to_render), 3);
    to_render->mat[to_render->width - 3][0] = x1;
    to_render->mat[to_render->width - 3][1] = y1;
    to_render->mat[to_render->width - 3][2] = z1;
    to_render->mat[to_render->width - 3][3] = 1;
    to_render->mat[to_render->width - 2][0] = x2;
    to_render->mat[to_render->width - 2][1] = y2;
    to_render->mat[to_render->width - 2][2] = z2;
    to_render->mat[to_render->width - 2][3] = 1;
    to_render->mat[to_render->width - 1][0] = x3;
    to_render->mat[to_render->width - 1][1] = y3;
    to_render->mat[to_render->width - 1][2] = z3;
    to_render->mat[to_render->width - 1][3] = 1;
}

void draw_triangles(matrix * to_render, struct point eye, Uint32 color){
    int startX = 6;
    if(to_render->width <= 4) return;
    //matrix move_to_zero = translation_matrix( (sxr
    /*
    printf("Printing points 1, 2, 3:\n");
    print_point(p1);
    print_point(p2);
    print_point(p3);*/
    //printf("Did not get culled\n");
    struct point p1, p2, p3;
    while( startX < to_render->width ) {
        p1.x = to_render->mat[startX-2][0];
        p1.y = to_render->mat[startX-2][1];
        p1.z = to_render->mat[startX-2][2];
        p2.x = to_render->mat[startX-1][0];
        p2.x = to_render->mat[startX-1][1];
        p2.x = to_render->mat[startX-1][2];
        p3.x = to_render->mat[startX][0];
        p3.x = to_render->mat[startX][1];
        p3.x = to_render->mat[startX][2];
        if(get_direction(p1, p2, p3, eye) <= 0) {
            startX += 3;
            continue;
        }

        draw_line( to_render->mat[startX - 2][0], to_render->mat[startX - 2][1],
                to_render->mat[startX - 1][0], to_render->mat[startX - 1][1], color);
        draw_line( to_render->mat[startX - 2][0], to_render->mat[startX - 2][1],
                to_render->mat[startX][0], to_render->mat[startX][1], color);
        draw_line( to_render->mat[startX-1][0], to_render->mat[startX-1][1],
                to_render->mat[startX][0], to_render->mat[startX][1], color);       
        startX += 3;
    }
}

void render_to_eye( double ex, double ey, double ez, matrix * to_render) {
    int i;
    for( i = 4; i < to_render->width; i++ ) {
        //edge.mat[i][0] = -ez * ( (edge.mat[i][0] - ex) / edge.mat[i][2] - ez ) + ex;
        //edge.mat[i][1] = (edge.mat[i][2] - ez) / ( -ez ) * ( edge.mat[i][1] - ey ) + ey;  
        to_render->mat[i][0] = (-ez / (to_render->mat[i][2] - ez) ) * ( to_render->mat[i][0] - ex) + ex;
        to_render->mat[i][1] = (-ez / (to_render->mat[i][2] - ez) ) * ( to_render->mat[i][1] - ey) + ey;
        to_render->mat[i][0] = (width)  * (to_render->mat[i][0] - sxl) / (sxr - sxl);
        to_render->mat[i][1] = height - ( (height) * (to_render->mat[i][1] - syl) / (syr - syl) );
    }

}
void convert_from_screen(matrix * to_render) {
    int i;

    for( i = 4; i < to_render->width; i++ ) {
        //double before = to_render->mat[i][0] - (sxl);
        to_render->mat[i][0] = (width)  * (to_render->mat[i][0] - sxl) / (sxr - sxl); //Alter the x

        // before = to_render->mat[i][1] - (syl);
        to_render->mat[i][1] = height - ( (height) * (to_render->mat[i][1] - syl) / (syr - syl) );
        //to_render->mat[i][1] = height * (before / (syr - syl));
        //to_render->mat[i][1] = height - to_render->mat[i][1];
    }
}

