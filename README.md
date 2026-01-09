# Computer Science 305 via Study.com - Assignment 2: Building Simple Operating System Functionality

## Required Tooling

> The project expects a Linux environment, tested in Ubuntu 24.04.3 LTS via WSL and Arch.

### CMake
- *CMake is the de-facto standard for building C++ code.* - [CMake.org](https://cmake.org/)
- Tested with version 4.2.1

### GCC
- *The GNU Compiler Collection includes front ends for C, C++, Objective-C, Fortran, Ada, Go, D, Modula-2, and COBOL as well as libraries for these languages (libstdc++,...).* - [gcc.gnu.org](https://gcc.gnu.org/)
- Tested with version 15.2

## Build Instructions

> Build and run automatically.
1. Clone the repository.
1. Run the build script: `./run.sh`

> Build and run manually. 
1. Clone the repository.
1. Create a build directory: `cmake -B build`
1. Build the project: `cmake --build build`
1. Run the application: `./build/bin/assignment2`

## Output

When running with the included `run.sh` shell script, the program will be executed and the output will be saved to a `.txt` file in the `output` directory as shown below.

Remaining execution time for running jobs is described by the number of `.` characters following the job state.  For example, a running job with 3 seconds of execution left is represented by `Running...`.

![Demo of running jobs.](screenshots/demo.png)

