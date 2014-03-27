#include "line.h"
/*
int main(int argc, char ** argv) {
    init_background( "line.ppm" );
    int cols[3];
    cols[0] = 255;
    cols[1] = 255;
    cols[2] = 255;
    //draw_line(0, 0, 400, 400, cols);
    //draw_line(40, 0, 10, 100, cols);
    //draw_line(4, 5, 6, 7, cols);
    draw_line(90, 40, 50, 90, cols);
    draw_line(200, 12, 200, 400, cols);

}
**/
void draw_line(int x1, int y1, int x2, int y2) {
    int xdiff = x1 - x2;
    int ydiff = y1 - y2;
    SDL_Color s;
    s.r = 255;
    s.g = 255;
    s.b = 255;

    if( !xdiff && !ydiff ) {
        //Given only one point
        //color_pixel(x1, y1, cols);
        drawPixel(x1, y1, *(Uint32*)&s);
        return;
    }
    //Pick major direction
    int major = XMAJOR;
    if( abs(ydiff) > abs(xdiff) ) major = YMAJOR;
    //Swap the points if the major direction goes decrementally
    if( (major == XMAJOR) ? xdiff > 0 : ydiff > 0 ) {
        swap(x1, x2);
        swap(y1, y2);
    }
    //Color pixels
    int max = (major == XMAJOR) ? ( x2 - x1 ) : (y2 - y1);
    int min = (major != XMAJOR) ? ( x2 - x1 ) : (y2 - y1);
    int acc = 0;
    int inc = ( min < 0 ) ? -1 : 1;
    //printf("x1 x2 y1 y2 dirr%d %d %d %d %d\n", x1, x2, y1, y2, (major == XMAJOR));
    while( (major == XMAJOR) ? x1 <= x2 : y1 <= y2 ) {

        drawPixel(x1, y1, *(Uint32*)&s);
        acc += abs(min);
        if(acc >= max) {
            acc = acc % max;
            if(major == XMAJOR) y1 += inc; 
            else x1 += inc;
        }
        if(major == XMAJOR) x1++; 
        else y1++;

    }
}
/*
void color_pixel(int x, int y) {
    printf("Coloring pixel (x, y) : %d %d\n", x, y);
}*/
