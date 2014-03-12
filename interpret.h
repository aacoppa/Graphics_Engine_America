#ifndef __INTERP_ACG
#define __INTERP_ACG 1

#include <stdio.h>
#include <stdlib.h>     
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "background.h"
#include "matrix.h"
#include "parse_util.h"
#include "line.h"
#include "transformations.h"
#include "assert.h"

#define COLOR           1
#define LINE            2
#define NAME            3
#define QUIT            4
#define NOT_READY       5
#define ERROR           6
#define COMMENT         7
#define ROTATE_X        8
#define ROTATE_Y        9
#define ROTATE_Z        10
#define TRANSLATE       11
#define SCALE           12
#define SCREEN          13
#define PIXELS          14    
#define TRANSFORM       15
#define RENDER_PARALLEL 16
#define RENDER_CYCLOPS  17
#define RENDER_STEREO   18
#define IDENTITY        19
#define CLEAR_PIXELS    20
#define CLEAR_EDGES     21

char * fn;
FILE * fp;
int * curr_cols;
matrix edge;
matrix transformer;
char ** args;
double sxl, syl, sxr, syr; //Bottom left, top rigt

void draw_lines();
void add_line_to_edge();
void init();
void convert_from_screen();
int next_type();
int handle_type();
char * next_data();
int * get_points();
#endif
