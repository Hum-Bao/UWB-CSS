#!/bin/bash

rm -f a.out

#cp BFSDISK-clean-backup BFSDISK

gcc -fcommon -Wall -Wextra -Wno-sign-compare *.c

./a.out
