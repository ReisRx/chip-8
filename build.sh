#!/bin/bash

if [[ $1 == "debug" ]]; then
    gcc -g main.c chip8.c -o build/debug.exe -Ilibs/include -Llibs/lib -lSDL2 -lSDL2_image -lm
else
    gcc main.c chip8.c -o build/chip8.exe -Ilibs/include -Llibs/lib -lSDL2 -lSDL2_image -lm
fi