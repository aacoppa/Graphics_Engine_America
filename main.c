#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>

int main(int argc, char ** argv) {
    SDL_Init( SDL_INIT_EVERYTHING );
    SDL_Surface* screen = NULL;
    screen = SDL_SetVideoMode( 640, 480, 32, SDL_SWSURFACE );
    SDL_Delay( 2000 );
    //Quit SDL
    SDL_Quit();
    return 0;
}
