# Intel 8080 Emulator
## About
A C++-written emulator for the 8-bit Intel 8080 Microprocessor, launched in Q2 1974, with all documented opcodes with rudimentary clock cycle emulation.
## Usage
This lacks command-line input for loading files into memory, so requires editing of the loadMem thread, which requires recompilation.
## Requirements
- Boost Thread
- Boost Chrono
- GCC 13
## Build
GCC:
```gcc mainboard.cpp processor/processor.cpp -o i8080 -std=c++17 -lstdc++ -lboost_thread -lboost_chrono```
