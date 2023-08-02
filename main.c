#define SDL_MAIN_HANDLED
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#include "chip8.h"

extern unsigned char gfx[WIDTH][HEIGHT];
extern unsigned char drawFlag;

void drawFromGFX(SDL_Renderer* renderer) {
    for(unsigned char row = 0; row < WIDTH; row++) {
        for(unsigned char column = 0; column < HEIGHT; column++) {
            if(gfx[row][column] == 0) { // Black
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            } else { // White
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }

            SDL_Rect rect = {column * CELL_SIZE, row * CELL_SIZE, CELL_SIZE, CELL_SIZE};
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}
 
int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }

    SDL_Window* window = SDL_CreateWindow("CHIP-8",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE, 0);
    
    if(window == NULL) {
        printf("Error creating window: %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }


    chip8_Initialize();

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Event event;
    unsigned char quit = 0;

    while (!quit) {
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

        if(drawFlag) {
            drawFromGFX(renderer);
            SDL_RenderPresent(renderer);
            drawFlag = 0;
        }
        // SDL_Delay(500);
        SDL_Delay(250);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}