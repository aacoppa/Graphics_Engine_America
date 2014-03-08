/* Generates the ppm:
 * usage:
 *  init_background( width, height );
 *  //Draw a bunch of lines
 *  write_array( file_name );
 */
#ifndef __BACKGROUND_ACG
#define __BACKGROUND_ACG 1

#include <stdio.h>
#include <stdlib.h>     
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

//#define WIDTH 500
//#define HEIGHT 500

#define BYTES_PER_COLOR 16

#define START_BYTES 15
#define BYTES_PER_LINE ( BYTES_PER_COLOR * WIDTH + 1 )

void color_pixel(int, int, int []);
int * random_colors();
void init_background();
void write_array( char * );
int fd;
int width, height; //Too lazy to lowercase where WIDTH/HEIGHT is used, forgive me C gods...
typedef struct color {
    int cols[3];
} color;

color ** pixels;
#endif
