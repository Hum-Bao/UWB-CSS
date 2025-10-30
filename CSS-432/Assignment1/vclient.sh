g++ client.cpp -o client
valgrind ./client 13579 20000 $1 $2 127.0.0.1 $3
