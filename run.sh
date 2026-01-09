#!/bin/bash

# Create build directory and configure cmake if `build` does not exist.
if [[ ! -d build ]]; then
    cmake -B build
fi

# Build project.
cmake --build build

# Run project for each algorithm, outputting to file.
#./build/bin/assignment2 > output/Program-Output.txt
./build/bin/assignment2

# Status code, info dump, and exit.
#echo "Program exited with code $?.  Output written to 'output' directory."

