#ifndef __CHIP8_H__
#define __CHIP8_H__

#include <SDL2/SDL.h>

#define WIDTH 64
#define HEIGHT 32
#define CELL_SIZE 20
#define MEMORY_SIZE 4096
#define STACK_SIZE 16
#define KEYPAD_SIZE 16
#define REGISTERS_SIZE 16


void chip8_Initialize();
void chip8_LoadGame(char *game);
void chip8_EmulateCycle();
void chip8_Tick();


const static unsigned char KEYMAP[KEYPAD_SIZE] = {
    SDLK_x, // 0 
    SDLK_1, // 1
    SDLK_2, // 2
    SDLK_3, // 3
    SDLK_q, // 4
    SDLK_w, // 5
    SDLK_e, // 6
    SDLK_a, // 7
    SDLK_s, // 8
    SDLK_d, // 9
    SDLK_z, // A
    SDLK_c, // B
    SDLK_4, // C
    SDLK_r, // D
    SDLK_f, // E
    SDLK_v  // F
};

#endif