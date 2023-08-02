#define SDL_MAIN_HANDLED
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#include "chip8.h"

SDL_Window* setupGraphics() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("Error initializing SDL: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_Window* win = SDL_CreateWindow("CHIP-8",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       640, 320, 0);
    
    return win;
}
 
int main(int argc, char *argv[])
{
    // setupGraphics();
    // setupInput();

    // returns zero on success else non-zero
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }

    SDL_Window* window = SDL_CreateWindow("CHIP-8",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       640, 320, 0);
    
    if(window == NULL) {
        printf("Error creating window: %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }


    chip8_Initialize();

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Event event;
    unsigned char quit = 0;

    while (!quit) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Handling Input
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    quit = 1;
                }
            }  
        }

        // Continue emulation

        chip8_EmulateCycle();

        SDL_RenderPresent(renderer);
        SDL_Delay(500);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}