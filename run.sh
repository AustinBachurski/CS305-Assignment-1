#!/bin/bash

# Create build directory and configure cmake if `build` does not exist.
if [[ ! -d build ]]; then
    cmake -B build
fi

# Build project.
cmake --build build

# Run project for each algorithm, outputting to file.
./build/bin/assignment1 --First-Fit > output/First-Fit.txt
./build/bin/assignment1 --Best-Fit > output/Best-Fit.txt
./build/bin/assignment1 --Worst-Fit > output/Worst-Fit.txt

# Status code, info dump, and exit.
echo "Program exited with code $?.  Output written to 'output' directory."

