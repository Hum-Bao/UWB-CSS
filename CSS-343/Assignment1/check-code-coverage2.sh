#!/bin/bash
PROG=$0
EXE="test_exec"
CC=g++

programs=($CC "gcov")
for p in "${programs[@]}"; 
do
    if ! hash $CC 2>/dev/null; 
    then
        echo "ERROR: $PROG: cannot find $CC executable"
        exit 1
    fi
done

$CC -fprofile-arcs -ftest-coverage -g -fPIC -O0 $(dirname $0)/*.cpp -o $(dirname $0)/$EXE

if [ ! -f $EXE ]; 
then
    echo "ERROR: $PROG: Failed to create executable"
    exit 1
fi

# Execute the program
if [ -z $1 ];
then
    ./$EXE
else
    ./$EXE $1
fi 


gcovr -r .

rm $EXE *.gcno *.gcda