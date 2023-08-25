# CHIP-8

### how to run:

    $ gcc main.c chip8.c -o build/test.exe -Ilibs/include -Llibs/lib -lSDL2 -lSDL2_image

### for debug:

    $ gcc -g main.c chip8.c -o build/debug.exe -Ilibs/include -Llibs/lib -lSDL2 -lSDL2_image

### To Do:

- [ ] Makefile
- [ ] Linux .so files
- [x] Opcodes:
    - [x] 00E0
    - [x] 00EE
    - [x] 1NNN
    - [x] 2NNN
    - [x] 3XNN
    - [x] 4XNN
    - [x] 5XY0
    - [x] 6XNN
    - [x] 7XNN
    - [x] 8XY0
    - [x] 8XY1
    - [x] 8XY2
    - [x] 8XY3
    - [x] 8XY4
    - [x] 8XY5
    - [x] 8XY6
    - [x] 8XY7
    - [x] 8XYE
    - [x] 9XY0
    - [x] ANNN
    - [x] BNNN
    - [x] CXNN
    - [x] DXYN
    - [x] EX9E
    - [x] EXA1
    - [x] FX07
    - [x] FX0A
    - [x] FX15
    - [x] FX18
    - [x] FX1E
    - [x] FX29
    - [x] FX33
    - [x] FX55
    - [x] FX65
- [x] fix timings (emulate hardware)
- [x] remove prints or add DEBUG flag in makefile to print

- Credits:
    - https://github.com/Timendus/chip8-test-suite for testing
    - https://tobiasvl.github.io/blog/write-a-chip-8-emulator/#timing guide
    - https://multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/ guide
    - https://youtu.be/YtSgV3gY3fs?si=Ltyd1Jb9P9975EZj&t=360 special thanks for time step part