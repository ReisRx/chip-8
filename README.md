# CHIP-8

CHIP-8 is usually called an emulator but it's basically an interpreted programming language made in the 1970s. ([Wiki](https://en.wikipedia.org/wiki/CHIP-8#))

I decided to do this because it's said to be a great starting point on Emulation.  

## How to run:

##### Normal:

    $ ./build.sh

##### Debug:

    $ ./build.sh debug


## About the code

I made this in C using GCC (version 13.1.0) and SDL2 (version 2.28.2). Its been tested to run on Windows and Linux (Arch Linux).

## References:

##### Guides and Wikis:
- https://tobiasvl.github.io/blog/write-a-chip-8-emulator/#timing
- https://multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/
- https://en.wikipedia.org/wiki/CHIP-8
- https://wiki.libsdl.org/SDL2/CategoryAPI

##### Special Thanks:
- https://github.com/Timendus/chip8-test-suite
- https://youtu.be/YtSgV3gY3fs?si=Ltyd1Jb9P9975EZj&t=360 (Emulating cycles)
