#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chip8.h"

unsigned char fontset[80] = 
{ 
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F 
};

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
    memset(keypad, 0, sizeof(unsigned char) * KEYPAD_SIZE);

    delayTimer = 0;
    soundTimer = 0;

    drawFlag = 0;

    for(int i = 0; i < 80; i++) {
        memory[i] = fontset[i];
    }
}

void chip8_LoadGame(char *game) {
    FILE *gameFile = fopen(game, "rb");

    if (gameFile == NULL) {
        fprintf(stderr, "Didn't find the game: %s\n", game);
        exit(EXIT_FAILURE);
    }

    // instead of using 0xFFF - 0x200 as maxSize, use the fileSize
    fseek(gameFile, 0, SEEK_END);
    long fileSize = ftell(gameFile);
    rewind(gameFile);

    fread(memory + 0x200, 1, fileSize, gameFile);
    fclose(gameFile);
}

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
                    programCounter = stack[--stackPointer];
                    break;

                default:
                    // TODO: Implement unknown opcode
                    break;
            }
            break;

        case 0x1000: // 0x1NNN return
            programCounter = currentOpcode & 0x0FFF;
            break;

        case 0x2000: // 0x2NNN goto NNN
            stack[stackPointer++] = programCounter + 2;
            programCounter = currentOpcode & 0x0FFF;
            break;

        case 0x3000: // 0x3XNN if (VX == NN)
            if(registers[(currentOpcode & 0x0F00) >> 8] == (currentOpcode & 0x00FF)) {
                programCounter += 2;
            }
            programCounter += 2;
            break;
        
        
        case 0x4000: // 0x4XNN if (VX != NN)
            if(registers[(currentOpcode & 0x0F00) >> 8] != (currentOpcode & 0x00FF)) {
                programCounter += 2;
            }
            programCounter += 2;
            break;
        
        case 0x5000: // 0x5XY0 if (VX == VY)
            if (registers[(currentOpcode & 0x0F00) >> 8] == registers[(currentOpcode & 0x00F0) >> 4]) {
                programCounter += 2;
            }
            programCounter += 2;
            break;

        case 0x6000: // 0x6XNN VX = NN
            registers[(currentOpcode & 0x0F00) >> 8] = currentOpcode & 0x00FF;
            programCounter += 2;
            break;

        case 0x7000: // 0x7XNN VX += NN
            registers[(currentOpcode & 0x0F00) >> 8] += (currentOpcode & 0x00FF);
            programCounter += 2;
            break;

        case 0x8000: // 0x8XYN
            unsigned char registerX = (currentOpcode & 0x0F00) >> 8;
            unsigned char registerY = (currentOpcode & 0x00F0) >> 4;
            unsigned char vY = registers[registerY];
            unsigned char oldRegisterX = registers[registerX];

            switch (currentOpcode & 0x000F) {
                
                case 0x0: // VX = VY
                    registers[registerX] = vY;
                    programCounter += 2;
                    break;
                
                case 0x1: // VX |= VY
                    registers[registerX] |= vY;
                    registers[0xF] = 0;
                    programCounter += 2;
                    break;
                
                case 0x2: // VX &= VY
                    registers[registerX] &= vY;
                    registers[0xF] = 0;
                    programCounter += 2;
                    break;

                case 0x3: // VX ^= VY
                    registers[registerX] ^= vY;
                    registers[0xF] = 0;
                    programCounter += 2;
                    break;

                case 0x4: // VX += VY
                    registers[registerX] += vY;
                    registers[0xF] = registers[registerX] < oldRegisterX ? 1 : 0;
                    programCounter += 2;
                    break;
                
                case 0x5: // VX -= VY
                    registers[registerX] -= vY;
                    registers[0xF] = vY > oldRegisterX ? 0 : 1;
                    programCounter += 2;
                    break;

                case 0x6: // VX >>= 1
                    registers[registerY] >>= 1;
                    registers[registerX] = registers[registerY];
                    // registers[registerX] >>= 1;
                    registers[0xF] = oldRegisterX & 0x01; // less significant bit
                    programCounter += 2;
                    break;

                case 0x7: // VX = VY - VX
                    registers[registerX] = vY - registers[registerX];
                    registers[0xF] = oldRegisterX > vY ? 0 : 1;
                    programCounter += 2;
                    break;

                case 0xE: // VX <<= 1
                    registers[registerY] <<= 1;
                    registers[registerX] = registers[registerY];
                    // registers[registerX] <<= 1;
                    registers[0xF] = (oldRegisterX & 0x80) >> 7; // most significant bit
                    programCounter += 2;
                    break;
                
                default:
                    // TODO: Unknown opcode
                    break;
            }
            break;

        case 0x9000: // 0x9XY0 if (VX != VY)
            if(registers[(currentOpcode & 0x0F00) >> 8] != registers[(currentOpcode & 0x00F0) >> 4]) {
                programCounter += 2;
            }
            programCounter += 2;
            break;

        case 0xA000: // 0xANNN
            indexRegister = currentOpcode & 0x0FFF;
            programCounter += 2;
            break;

        case 0xB000: // 0xBNNN PC = V0 + NNN
            programCounter = registers[0x0] + (currentOpcode & 0x0FFF);
            break;

        case 0xC000: // 0xCXNN VX = rand() & NN
            unsigned char random = rand(); // forces to a max of 255
            registers[(currentOpcode & 0x0F00) >> 8] = random & (currentOpcode & 0x00FF);
            programCounter += 2;
            break;

        case 0xD000: // 0xDXYN
            unsigned char coordinateX = registers[(currentOpcode & 0x0F00) >> 8];
            unsigned char coordinateY = registers[(currentOpcode & 0x00F0) >> 4];

            unsigned char heightLimit = currentOpcode & 0x000F;

            unsigned char carryFlag = 0;

            for(unsigned char height = 0; height < heightLimit; height++) {
                unsigned char sprite = memory[indexRegister + height];
                
                for(unsigned char bitInRow = 0; bitInRow < 8; bitInRow++) {
                    if((sprite & (0x80 >> bitInRow)) != 0) {
                        if(gfx[(coordinateX + bitInRow) % WIDTH][(coordinateY + height) % HEIGHT] == 1) {
                            carryFlag = 1; // Vf = 1 (carry flag)
                        } else {
                            carryFlag = 0;
                        }

                        gfx[(coordinateX + bitInRow) % WIDTH][(coordinateY + height) % HEIGHT] ^= 1;
                        registers[0xF] = carryFlag;
                    }
                }    
            }

            programCounter += 2;
            drawFlag = 1;
            break;

        case 0xE000: // 0xEXNN
            switch(currentOpcode & 0x00FF) {
                case 0x9E: // 0xEX9E if(key() == VX)
                    if(keypad[registers[(currentOpcode & 0x0F00) >> 8]]) { // if key was pressed
                        programCounter += 2;
                    }
                    programCounter += 2;
                    break;
                
                case 0xA1: // 0xEXA1 if(key() != VX)
                    if(!keypad[registers[(currentOpcode & 0x0F00) >> 8]]) { // if key was pressed
                        programCounter += 2;
                    }
                    programCounter += 2;
                    break;

                default:
                    // TODO: Implement unknown opcode
                    break;
            }
            break;

        case 0xF000: // 0xFXNN
            switch (currentOpcode & 0x00FF) {
                case 0x07: // 0xFX07 VX = delayTimer
                    registers[(currentOpcode & 0x0F00) >> 8] = delayTimer;
                    programCounter += 2;
                    break;

                case 0x0A: // 0xFX0A VX = get_key()
                    unsigned char isPressed = 0;

                    for(unsigned char i = 0; i < KEYPAD_SIZE; i++) {
                        if(keypad[i]) {
                            registers[(currentOpcode & 0x0F00) >> 8] = i;
                            isPressed = 1;
                        }
                    }

                    if(!isPressed) {
                        break;
                    }

                    programCounter += 2;
                    break;

                case 0x15: // 0xFX15 delayTimer = VX
                    delayTimer = registers[(currentOpcode & 0x0F00) >> 8];
                    programCounter += 2;
                    break;

                case 0x18: // 0xFX18 soundTimer = VX
                    soundTimer = registers[(currentOpcode & 0x0F00) >> 8];
                    programCounter += 2;
                    break;

                case 0x1E: // 0xFX1E I += VX
                    indexRegister += registers[(currentOpcode & 0x0F00) >> 8];
                    programCounter += 2;
                    break;

                case 0x29: // 0xFX29 I = spriteAddress
                    indexRegister = registers[(currentOpcode & 0x0F00) >> 8] * 5; // bytesPerChar = 5
                    programCounter += 2;
                    break;

                case 0x33: // 0xFX33 store in I binary-coded decimal
                    unsigned char number = registers[(currentOpcode & 0x0F00) >> 8];
                    memory[indexRegister] = number / 100;
                    memory[indexRegister + 1] = (number % 100) / 10;
                    memory[indexRegister + 2] = number % 10;
                    programCounter += 2;
                    break;

                case 0x55: // 0xFX55 store from V0 to VX into I
                    for(unsigned char i = 0; i <= (currentOpcode & 0x0F00) >> 8; i++) {
                        memory[indexRegister++] = registers[i];
                    }
                    programCounter += 2;
                    break;

                case 0x65: // 0xFX65 fill V0 to VX from I
                    for(unsigned char i = 0; i <= (currentOpcode & 0x0F00) >> 8; i++) {
                        registers[i] = memory[indexRegister++];
                        
                    }
                    programCounter += 2;
                    break;

                default:
                    // TODO: Implement unknown opcode
                    break;
            }
            break;

        default:
            // TODO: Implement unknown opcode
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

void chip8_Tick();