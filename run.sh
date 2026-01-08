cmake --build build
./build/bin/assignment1 --First-Fit > output/First-Fit.txt
./build/bin/assignment1 --Best-Fit > output/Best-Fit.txt
./build/bin/assignment1 --Worst-Fit > output/Worst-Fit.txt
echo "Program exited with code $?.  Output written to 'output' directory."

