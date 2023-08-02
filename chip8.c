#include <stdio.h>
#include <string.h>

#include "chip8.h"

/*
0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
0x200-0xFFF - Program ROM and work RAM
*/
unsigned char memory[MEMORY_SIZE];

// V0 ... VE and last one is carry flag
unsigned char registers[REGISTERS_SIZE];

unsigned short indexRegister;
unsigned short programCounter;
unsigned short currentOpcode;
unsigned char gfx[HEIGHT][WIDTH];

unsigned char delayTimer;
unsigned char soundTimer;

unsigned short stack[STACK_SIZE];
unsigned short stackPointer;

unsigned char keypad[KEYPAD_SIZE];

void chip8_Initialize() {
    programCounter = 0x200;
    currentOpcode = 0;
    indexRegister = 0;
    stackPointer = 0;

    memset(memory, 0, sizeof(unsigned char) * MEMORY_SIZE);
    memset(registers, 0, sizeof(unsigned char) * REGISTERS_SIZE);
    memset(gfx, 0, sizeof(unsigned char) * HEIGHT * WIDTH);
    memset(stack, 0, sizeof(unsigned short) * STACK_SIZE);

    // FOR TESTING ONLY
    // TODO: ALL 0x8XYN NEED TESTING

    memory[0x200] = 0x61;
    memory[0x201] = 0x01;
    
    memory[0x202] = 0x62;
    memory[0x203] = 0x02;

    memory[0x204] = 0x63;
    memory[0x205] = 0x03;

    // TODO: Load fontset

    delayTimer = 0;
    soundTimer = 0;
}

// use fopen in binary mode and start writing from 0x200 + 1
void chip8_LoadGame(char *game);

void chip8_EmulateCycle() {
    // Fetch opcode
    currentOpcode = memory[programCounter] << 8 | memory[programCounter + 1];
    printf("curr = 0x%X\n", currentOpcode);

    // Decode and execute opcode
    switch(currentOpcode & 0xF000) {
        case 0x6000: // 0x6XNN VX = NN
            registers[(currentOpcode & 0x0F00) >> 8] = currentOpcode & 0x00FF;
            programCounter += 2;
            break;

        case 0x8000: // 0x8XYN
            unsigned char registerX = (currentOpcode & 0x0F00) >> 8;
            unsigned char registerY = (currentOpcode & 0x00F0) >> 4;

            switch (currentOpcode & 0x000F) {
                case 0x0: // VX = VY
                    registers[registerX] = registers[registerY];
                    break;
                
                case 0x1: // VX |= VY
                    registers[registerX] |= registers[registerY];
                    break;
                // TODO: Continue doing 0x8000 cases
            }
            break;
            print("Vx = 0x%x\n", registers[(currentOpcode & 0x0F00) >> 8]);
        default:
            break;
    }

    // Update timers
    if(delayTimer > 0) {
        delayTimer--;
    }

    if(soundTimer > 0) {
        printf("beep\n");
        soundTimer--;
    }
}

void chip8_SetKeys();

void chip8_Tick();