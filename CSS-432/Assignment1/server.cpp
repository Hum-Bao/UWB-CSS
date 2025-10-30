
#include <bits/types/struct_timeval.h>
#include <netdb.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <array>
#include <cstring>
#include <iostream>
#include <string>

const int BUFFER_SIZE = 1500;
const int CONNECTIONS = 5;

int repetitions = 0;

void* create_client_thread(void* client_socket_ptr) {
    //static_cast to convert void ptr to int ptr
    const int socket = *static_cast<int*>(client_socket_ptr);
    delete static_cast<int*>(client_socket_ptr);  // Clean up memory

    //Buffer array
    std::array<char, BUFFER_SIZE> databuff = std::array<char, BUFFER_SIZE>();

    struct timeval start {};

    struct timeval end {};

    // Get timestamp for connection start
    gettimeofday(&start, nullptr);

    // Get and log amount of reads
    int count = 0;
    for (int i = 0; i < repetitions; i++) {
        for (long read_num = 0;
             (read_num += read(socket, databuff.data(),
                               BUFFER_SIZE - read_num)) < BUFFER_SIZE;
             ++count)
            ;
        ++count;
    }

    //Send reads to client
    write(socket, &count, sizeof(count));

    // Gets the stop time
    gettimeofday(&end, nullptr);

    // Calculates the rtt.
    const long rtt =
        (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
    std::cout << "data-receiving time = " << rtt << " usec\n";
    close(socket);
    return nullptr;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Not enough args, currently have: " << argc - 1
                  << ", need 2\n";
        return 1;
    }

    // int port = atoi(argv[1]);
    const std::string port = argv[1];
    repetitions = std::stoi(argv[2]);

    //Struct for getaddrinfo
    struct addrinfo hints {};
    struct addrinfo* servinfo{};
    struct addrinfo* p{};
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;      // Allow IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;  // TCP stream sockets
    hints.ai_flags = AI_PASSIVE;      // Use my IP

    // //Uses getaddrinfo instead of depreciated gethostbyname
    if (getaddrinfo(nullptr, argv[1], &hints, &servinfo) != 0) {
        std::cerr << "getaddrinfo error" << std::endl;
        return 1;
    }

    std::cout << "Starting server on port: " << port << "\n";

    // Create a socket
    int server_sd;
    for (p = servinfo; p != nullptr; p = p->ai_next) {
        server_sd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (server_sd == -1) {
            continue;  // Try the next one
        }

        const int on = 1;
        if (setsockopt(server_sd, SOL_SOCKET, SO_REUSEADDR, (char*)&on,
                       sizeof(on)) == -1) {
            std::cerr << "setsockopt failed" << std::endl;
            close(server_sd);
            continue;  // Try the next one
        }

        // Bind the socket
        if (bind(server_sd, p->ai_addr, p->ai_addrlen) == -1) {
            close(server_sd);
            continue;  // Try the next one
        }
        break;  // Successfully bound
    }

    //Exit if failed to bind
    if (p == nullptr) {
        std::cerr << "Failed to bind the socket" << "\n";
        //Free addrinfo structure as client_sd is connected
        freeaddrinfo(servinfo);
        return -1;
    }

    //Free addrinfo structure as client_sd is connected
    freeaddrinfo(servinfo);

    // Start listening
    if (listen(server_sd, CONNECTIONS) == -1) {
        std::cerr << "Listen error" << "\n";
        close(server_sd);
        return -1;
    }

    // Accept connections until the process is killed
    while (true) {
        sockaddr_storage new_sock_addr{};
        socklen_t new_sock_addr_size = sizeof(new_sock_addr);

        // Accepting connections
        int* new_sd =
            new int;  // Dynamically allocate memory for the new socket
        *new_sd =
            accept(server_sd, (sockaddr*)&new_sock_addr, &new_sock_addr_size);
        if (*new_sd == -1) {
            std::cerr << "Accept error" << "\n";
            delete new_sd;  // Clean up memory if accept fails
            continue;
        }

        std::cout << "new_sd=" << *new_sd << "\n";

        // Create the thread to run the create_client_thread function with the new_sd
        pthread_t thread;
        if (pthread_create(&thread, nullptr, create_client_thread, new_sd) <
            0) {
            std::cerr << "Thread not created. \n";
            close(*new_sd);
            delete new_sd;  // Clean up memory if thread creation fails
        }

        // Detach the thread to allow it to clean up after itself
        pthread_detach(thread);
    }

    close(server_sd);  // Close the server socket
    return 0;
}