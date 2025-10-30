
#start webserver in the background and pause for a moment while it loads
valgrind --leak-check=full --log-file="webserver.out" ./webserver &
pid=$!
sleep 5

echo "TEST CASE 2: Retriever accessing real server" 

./retriever www.neverssl.com/

echo "TEST CASE 3: Retriever accessing my server"
./retriever 127.0.0.1/index.html

echo "TEST CASE 4: Retriever sending an unallowed method to my server"
./retriever 127.0.0.1/index.html unallowed

echo "TEST CASE 5: Retriever requesting a forbidden file"
./retriever 127.0.0.1/passwords.txt

echo "TEST CASE 6: Retriever requesting a non-existent file"
./retriever 127.0.0.1/notrealfile.pcap

echo "TEST CASE 7: Retriever sending a malformed request"
./retriever 127.0.0.1/index.html malformed

echo "TEST CASE 8: Retriever Valgrind leak check"
valgrind --leak-check=full --log-file="retriever.out" ./retriever 127.0.0.1/index.html
cat retriever.out
rm retriever.out

echo "TEST CASE 9: Webserver Valgrind leak check"
kill $pid
sleep 5
cat webserver.out
sleep 5
rm webserver.out


# shut down the webserver so it isn't running when we do this again.
#killall webserver
