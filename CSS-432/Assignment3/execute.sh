#Build cpp files
bash build.sh

#Case 1
./server 1 &
sleep 0.5
./client 1 127.0.0.1 &

sleep 5

#Case 2
./server 2 &
sleep 0.5
./client 2 127.0.0.1

sleep 5

#Case 3
./server 3 &
sleep 0.5
./client 3 127.0.0.1

sleep 5

#Case 1 Valgrind
valgrind --leak-check=full ./server 1 &
sleep 2
valgrind --leak-check=full ./client 1 127.0.0.1

sleep 5

#Case 2 Valgrind
valgrind --leak-check=full ./server 2 &
sleep 2
valgrind --leak-check=full ./client 2 127.0.0.1

sleep 5

#Case 3 Valgrind
valgrind --leak-check=full ./server 3 &
sleep 2
valgrind --leak-check=full ./client 3 127.0.0.1

#Benchmark
#./server 4 &
#./client 4 127.0.0.1
