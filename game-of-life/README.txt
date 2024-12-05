Game of Life
============

This little project implements the Conway's Game of Life. For rendering, input handling, and window
handling we use SDL2 so ensure that the its headers are available in your computer.

Build & Run
------------

For building the program, ensure you have `cmake` available and run the following commands in the
project directory:

    cmake -S . -B build
    cmake --build build

you can then run ./build/bin/gol to run the program.
