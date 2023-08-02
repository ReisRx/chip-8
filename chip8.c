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
unsigned char gfx[WIDTH][HEIGHT];

unsigned char delayTimer;
unsigned char soundTimer;

unsigned short stack[STACK_SIZE];
unsigned short stackPointer;

unsigned char keypad[KEYPAD_SIZE];

unsigned char drawFlag;


void chip8_Initialize() {
    programCounter = 0x200;
    currentOpcode = 0;
    indexRegister = 0;
    stackPointer = 0;

    memset(memory, 0, sizeof(unsigned char) * MEMORY_SIZE);
    memset(registers, 0, sizeof(unsigned char) * REGISTERS_SIZE);
    memset(gfx, 0, sizeof(unsigned char) * HEIGHT * WIDTH);
    memset(stack, 0, sizeof(unsigned short) * STACK_SIZE);

    delayTimer = 0;
    soundTimer = 0;

    drawFlag = 0;

    // FOR TESTING ONLY
    // TODO: ALL 0x8XYN NEED TESTING

    memory[512] = 0x60;
    memory[513] = 0x18;
    
    memory[514] = 0x61;
    memory[515] = 0x18;

    memory[516] = 0x62;
    memory[517] = 0x18;

    memory[518] = 0x63;
    memory[519] = 0x24;

    memory[520] = 0xA5;
    memory[521] = 0x00;

    memory[522] = 0xF3;
    memory[523] = 0x55;

    memory[524] = 0x64;
    memory[525] = 0x00;

    memory[526] = 0xD4;
    memory[527] = 0x44;
    
    memory[528] = 0x00;
    memory[529] = 0xE0;

    // TODO: Load fontset


}

// use fopen in binary mode and start writing from 0x200 + 1
void chip8_LoadGame(char *game);

void chip8_EmulateCycle() {
    // Fetch opcode
    currentOpcode = memory[programCounter] << 8 | memory[programCounter + 1];
    printf("curr = 0x%X\n", currentOpcode);


    // Decode and execute opcode
    switch(currentOpcode & 0xF000) {
        case 0x0000: // 0x0NNN
            switch (currentOpcode) {
                case 0x00E0: // clear screen
                    memset(gfx, 0, sizeof(unsigned char) * HEIGHT * WIDTH);
                    programCounter += 2;
                    drawFlag = 1;
                    break;

                case 0x00EE: // return
                    // TODO:
                    break;

                default: // 0x0NNN
                    // TODO:
                    break;
            }
            break;

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
                    programCounter += 2;
                    break;
                
                case 0x1: // VX |= VY
                    registers[registerX] |= registers[registerY];
                    programCounter += 2;
                    break;
                // TODO: Continue doing 0x8000 cases
            }
            printf("Vx = 0x%x\n", registers[(currentOpcode & 0x0F00) >> 8]);
            break;

        case 0xA000: // 0xANNN
            indexRegister = currentOpcode & 0x0FFF;
            programCounter += 2;
            printf("I = 0x%x\n", indexRegister);
            break;

        case 0xD000: // 0xDXYN
            printf("drawing...\n");
            unsigned char coordinateX = registers[(currentOpcode & 0x0F00) >> 8];
            unsigned char coordinateY = registers[(currentOpcode & 0x00F0) >> 4];

            unsigned char rowLimit = currentOpcode & 0x000F;

            for(unsigned char row = 0; row < rowLimit; row++) {
                unsigned char sprite = memory[indexRegister + row];
                
                for(unsigned char column = 0; column < 8; column++) {
                    if((sprite & (0x80 >> column)) != 0) {
                        if(gfx[coordinateX + row][coordinateY + column] == 1) {
                            registers[0xF] = 1; // Vf = 1 (carry flag)
                        }

                        gfx[coordinateX + row][coordinateY + column] ^= 1;
                    }
                }    
            }

            programCounter += 2;
            drawFlag = 1;
            break;

        case 0xF000: // 0xFXNN
            switch (currentOpcode & 0x00FF) {
                case 0x55: // store from V0 to Vx
                    for(unsigned char i = 0; i <= (currentOpcode & 0x0F00) >> 8; i++) {
                        memory[indexRegister + i] = registers[i];
                        printf("memory[%d] = 0x%x\n", indexRegister + i, memory[indexRegister + i]);
                    }
                    programCounter += 2;
                    break;
            }
            break;

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