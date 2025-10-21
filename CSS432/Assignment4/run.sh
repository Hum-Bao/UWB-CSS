#!/bin/bash
bash build.sh
valgrind --leak-check=full --log-file="valgrind.out" ./switch > output.txt
cat valgrind.out
echo -e "\nSwitch output directed to output.txt"
#./switch
