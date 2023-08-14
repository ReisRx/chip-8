# CHIP-8

### how to run:

    $ gcc main.c chip8.c -o build/test.exe -Ilibs/include -Llibs/lib -lSDL2 -lSDL2_image

### for debug:

    $ gcc -g main.c chip8.c -o build/debug.exe -Ilibs/include -Llibs/lib -lSDL2 -lSDL2_image

### To Do:

- [ ] Makefile
- [ ] Opcodes:
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
    - [x] 8XY0
    - [x] 8XY1
    - [x] 8XY2
    - [x] 8XY3
    - [x] 8XY4
    - [x] 8XY5
    - [x] 8XY6
    - [x] 8XY7
    - [x] 8XYE
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
- [ ] remove prints or add DEBUG flag in makefile to print