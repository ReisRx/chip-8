#define SDL_MAIN_HANDLED
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#include "chip8.h"

extern unsigned char keypad[KEYPAD_SIZE];
extern unsigned char gfx[WIDTH][HEIGHT];
extern unsigned char drawFlag;
extern SDL_AudioDeviceID audioDevice;

void audioCallback(void *userdata, Uint8 *stream, int len) {
    double frequency = 440.0;
    double amplitude = 0.5;

    double angularFrequency = 2.0 * M_PI * frequency / 44100.0;

    for (int i = 0; i < len; i += 2) {
        Sint16 sample = (Sint16)(amplitude * 32767.0 * sin(angularFrequency * i));
        stream[i] = sample & 0xFF; 
        stream[i + 1] = (sample >> 8) & 0xFF;
    }
}

void drawFromGFX(SDL_Renderer* renderer) {
    for(unsigned char row = 0; row < WIDTH; row++) {
        for(unsigned char column = 0; column < HEIGHT; column++) {
            if(gfx[row][column] == 0) { // Black
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            } else { // White
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }

            SDL_Rect rect = {row * CELL_SIZE, column * CELL_SIZE, CELL_SIZE, CELL_SIZE};
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "No game loaded: ./executable <path-to-game>\n");
        exit(EXIT_FAILURE);
    }

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

    SDL_AudioSpec audioSpec;
    audioSpec.freq = 44100;
    audioSpec.format = AUDIO_S16SYS;
    audioSpec.channels = 1;
    audioSpec.samples = 1024;
    audioSpec.callback = audioCallback;
    audioSpec.userdata = NULL;

    audioDevice = SDL_OpenAudioDevice(NULL, 0, &audioSpec, NULL, 0);

    srand(time(0));
    chip8_Initialize();
    chip8_LoadGame(argv[1]);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Event event;
    unsigned char quit = 0;

    double start = 0;
    double end = (1 / 60.0) * CLOCKS_PER_SEC;
    double deltaTime = 0;
    double timeAccumulator = 0;
    double threshold = (1 / 60.0) * CLOCKS_PER_SEC;

    while (!quit) {
        deltaTime = end - start;
        start = clock();
        timeAccumulator += deltaTime;

        // Handling Input
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    quit = 1;
                }
            }  

            if(event.type == SDL_KEYDOWN) {
                for(unsigned char i = 0; i < KEYPAD_SIZE; i++) {
                    if(event.key.keysym.sym == KEYMAP[i]) {
                        keypad[i] = 1;
                    }
                }
            }

            if(event.type == SDL_KEYUP) {
                for(unsigned char i = 0; i < KEYPAD_SIZE; i++) {
                    if(event.key.keysym.sym == KEYMAP[i]) {
                        keypad[i] = 0;
                    }
                }
            }
        }

        // Continue emulation 
        if (timeAccumulator >= threshold){
            chip8_EmulateCycle();

            if(drawFlag) {
                drawFromGFX(renderer);
                SDL_RenderPresent(renderer);
                drawFlag = 0;
            }

            timeAccumulator -= threshold;
        }

        end = clock();
    }

    SDL_CloseAudioDevice(audioDevice);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}