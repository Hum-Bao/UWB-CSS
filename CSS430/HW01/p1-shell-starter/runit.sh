#!/bin/bash

# remove executable
echo "Removing previous executable: rm -f ./a.out"
rm -f ./a.out

# compile
echo "Compiling: gcc -Wall -Wextra *.c"
gcc -Wall -Wextra *.c

# execute
echo "Executing: ./a.out"
./a.out
