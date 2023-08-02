#ifndef __CHIP8_H__
#define __CHIP8_H__

#define WIDTH 64
#define HEIGHT 32
#define CELL_SIZE 10
#define MEMORY_SIZE 4096
#define STACK_SIZE 16
#define KEYPAD_SIZE 16
#define REGISTERS_SIZE 16


void chip8_Initialize();
void chip8_LoadGame(char *game);
void chip8_EmulateCycle();
void chip8_SetKeys();
void chip8_Tick();

#endif