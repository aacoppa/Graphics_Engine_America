#include "background.h"
int * random_colors() {
    int * cols = malloc(sizeof(int) * 3);
    int i; 
    for(i = 0; i < 3; i++) cols[i] = rand() % 255;
    return cols;
}
void init_background(int w, int h) {
    WIDTH = w;
    HEIGHT = h;
    pixels = malloc( WIDTH * sizeof(int *));
    for(int i = 0; i < WIDTH; i++) pixels[i] = malloc( HEIGHT * sizeof(color) );
}
void write_array(char * fn) {
    *(strchr(fn, '\n')) = '\0';
    fd = open(fn, O_CREAT | O_TRUNC | O_RDWR, 0666);
    char * header = "P3\n500 500 255\n"; //START_BYTES is the header size
    write(fd, header, strlen(header));
    int i = 0;
    while( i < HEIGHT ) {
        int j = 0;
        while( j < WIDTH ) {
            char * color = malloc(BYTES_PER_COLOR);
            int k;
            for(k = 0; k < 3; k++) {
                char c[4];
                sprintf(c, "%d", pixels[j][i].cols[k]);
                strcat(color, c);
                strcat(color, " ");
            }
            write(fd, color, BYTES_PER_COLOR);
            j++;
        }
        write(fd, "\n", 1);
        i++;
    }
}
void color_pixel(int x, int y, int cols []) {
    /*
    if( x >= WIDTH || x < 0 || y >= HEIGHT || y < 0) return;
    int off = START_BYTES + y * BYTES_PER_LINE;
    off = off + x * BYTES_PER_COLOR;
    lseek(fd, off, SEEK_SET);
    char * out = malloc(BYTES_PER_COLOR);
    int k;
    for(k = 0; k < 3; k++) {
        char c[4];
        sprintf(c, "%d", cols[k]);
        strcat(out, c);
        strcat(out, " ");
    }
    write(fd, out, BYTES_PER_COLOR);
    */
    for(int i = 0; i < 3; i++) pixels[x][y].cols[i] = cols[i];
}
