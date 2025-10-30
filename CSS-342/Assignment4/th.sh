#!/bin/bash

rm score.txt
touch score.txt

for i in {1..9}
do
	echo "TEST" $i
	echo "TEST" $i >> score.txt
	rm test$i 2> /dev/null
	g++ -o test$i test$i.cpp 
	./test$i >> score.txt
done
echo " "
echo "FINAL SCORE::"
grep SCORE score.txt | awk '{sum+=$2;}END{print sum;}'

for i in {1..9}
do
	rm test$i 2> /dev/null
done
