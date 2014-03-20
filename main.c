#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>

int main(int argc, char ** argv) {
    SDL_Init( SDL_INIT_EVERYTHING );
    
    //Quit SDL
    SDL_Quit();
    return 0;
}
