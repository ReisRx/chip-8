# CHIP-8

### how to run:

    $ gcc main.c chip8.c -o build/test.exe -Ilibs/include -Llibs/lib -lSDL2 -lSDL2_image

### for debug too:

    $ gcc main.c chip8.c -o build/test.exe -Ilibs/include -Llibs/lib -lSDL2 -lSDL2_image ; gcc -g main.c chip8.c -o build/debug.exe -Ilibs/include -Llibs/lib -lSDL2 -lSDL2_image

### To Do:

- Opcodes:
    - [ ] 0NNN
    - [x] 00E0
    - [ ] 00EE
    - [ ] 1NNN
    - [ ] 2NNN
    - [ ] 3XNN
    - [ ] 4XNN
    - [ ] 5XY0
    - [x] 6XNN
    - [ ] 7XNN
    - [x] 8XY0 (need testing)
    - [x] 8XY1 (need testing)
    - [ ] 8XY2
    - [ ] 8XY3
    - [ ] 8XY4
    - [ ] 8XY5
    - [ ] 8XY6
    - [ ] 8XY7
    - [ ] 8XYE
    - [ ] 9XY0
    - [x] ANNN
    - [ ] BNNN
    - [ ] CXNN
    - [x] DXYN
    - [ ] EX9E
    - [ ] EXA1
    - [ ] FX07
    - [ ] FX0A
    - [ ] FX15
    - [ ] FX18
    - [ ] FX1E
    - [ ] FX29
    - [ ] FX33
    - [x] FX55
    - [ ] FX65