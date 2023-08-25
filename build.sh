#!/bin/bash

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        if [[ $1 == "debug" ]]; then
            gcc -g main.c chip8.c -o build/debug -Ilibs/include -Llibs/lib -lSDL2 -lSDL2_image -lm
        else
            gcc main.c chip8.c -o build/chip8 -Ilibs/include -Llibs/lib -lSDL2 -lSDL2_image -lm
        fi
elif [[ "$OSTYPE" == "cygwin" ]]; then
        if [[ $1 == "debug" ]]; then
            gcc -g main.c chip8.c -o build/debug.exe -Ilibs/include -Llibs/lib -lSDL2 -lSDL2_image -lm
        else
            gcc main.c chip8.c -o build/chip8.exe -Ilibs/include -Llibs/lib -lSDL2 -lSDL2_image -lm
        fi
elif [[ "$OSTYPE" == "msys" ]]; then
        if [[ $1 == "debug" ]]; then
            gcc -g main.c chip8.c -o build/debug.exe -Ilibs/include -Llibs/lib -lSDL2 -lSDL2_image -lm
        else
            gcc main.c chip8.c -o build/chip8.exe -Ilibs/include -Llibs/lib -lSDL2 -lSDL2_image -lm
        fi
elif [[ "$OSTYPE" == "win32" ]]; then
        if [[ $1 == "debug" ]]; then
            gcc -g main.c chip8.c -o build/debug.exe -Ilibs/include -Llibs/lib -lSDL2 -lSDL2_image -lm
        else
            gcc main.c chip8.c -o build/chip8.exe -Ilibs/include -Llibs/lib -lSDL2 -lSDL2_image -lm
        fi
else
        echo "I don't support this OS. The command to run on other OS is gcc main.c chip8.c -o build/chip8.exe -Ilibs/include -Llibs/lib -lSDL2 -lSDL2_image -lm"
fi