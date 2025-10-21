#include <bits/types/struct_timeval.h>
#include <netdb.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <array>
#include <atomic>
#include <csignal>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>

//Atomic bool for allowing server to gracefully close on CTRL + C
std::atomic<bool> server_running(true);

//Signal handler to catch SIGINT (CTRL + C)
void handle_shutdown_signal(int signal) {
    if (signal == SIGINT || signal == SIGTERM) {
        std::cout << "SERVER: Shutdown signal received. Stopping server...\n";
        server_running = false;
    }
}

const int BUFFER_SIZE = 1024;
const int CONNECTIONS = 5;

//HTTP error types
const int HTTP_OKAY = 200;
const int HTTP_BAD = 400;
const int HTTP_FORBIDDEN = 403;
const int HTTP_NOT_FOUND = 404;
const int HTTP_METHOD_NOT_ALLOWED = 405;

std::string CreateHTTPResponse(int error_code) {
    switch (error_code) {
        case HTTP_OKAY: {
            return "HTTP/1.1 200 OK\r\n"
                   "Content-Type: text/html\r\n"
                   "\r\n";
        }
        case HTTP_FORBIDDEN: {
            return "HTTP/1.1 403 Forbidden\r\n"
                   "Content-Type: text/plain\r\n"
                   "Content-Length: 13\r\n"
                   "\r\n"
                   "403 Forbidden";
        }
        case HTTP_NOT_FOUND: {
            return "HTTP/1.1 404 Not Found\r\n"
                   "Content-Type: text/plain\r\n"
                   "Content-Length: 13\r\n"
                   "\r\n"
                   "404 Not Found";
        }

        case HTTP_METHOD_NOT_ALLOWED: {
            return "HTTP/1.1 405 Method Not Allowed\r\n"
                   "Content-Type: text/plain\r\n"
                   "Content-Length: 22\r\n"
                   "\r\n"
                   "405 Method Not Allowed";
        }

        default: {
            return "HTTP/1.1 400 Bad Request\r\n"
                   "Content-Type: text/plain\r\n"
                   "Content-Length: 15\r\n"
                   "\r\n"
                   "400 Bad Request";
        }
    }
}

void* create_client_thread(void* client_socket_ptr) {
    //static_cast to convert void ptr to int ptr
    const int socket = *static_cast<int*>(client_socket_ptr);
    delete static_cast<int*>(client_socket_ptr);  //Clean up memory

    //C++-style buffer array
    std::array<char, BUFFER_SIZE> buffer = std::array<char, BUFFER_SIZE>();

    //Read request
    ssize_t request_bytes = read(socket, buffer.data(), BUFFER_SIZE - 1);
    if (request_bytes < 0) {
        std::cerr << "SERVER: Error reading request\n";
        close(socket);
        return nullptr;
    }

    buffer[request_bytes] = '\0';
    std::string request(buffer.data());

    //Parse the HTTP method and path
    std::size_t method_end = request.find(' ');
    std::size_t path_end = request.find(' ', method_end + 1);

    //Check for a malformed request line
    if (method_end == std::string::npos || path_end == std::string::npos) {
        std::cerr << "SERVER: Malformed request\n";
        const std::string response = CreateHTTPResponse(HTTP_BAD);
        write(socket, response.c_str(), response.size());
        close(socket);
        return nullptr;
    }

    //Check for HTTP version mismatch
    const std::string http_version = request.substr(path_end + 1, 8);
    if (http_version != "HTTP/1.1" && http_version != "HTTP/1.0") {
        std::cerr << "SERVER: Invalid HTTP version: " << http_version << "\n";
        const std::string response = CreateHTTPResponse(HTTP_BAD);
        write(socket, response.c_str(), response.size());
        close(socket);
        return nullptr;
    }

    //Get the method and requested file
    std::string method = request.substr(0, method_end);
    std::string requested_file =
        request.substr(method_end + 1, path_end - method_end - 1);
    std::cout << "SERVER: File Requested: " << requested_file << "\n";

    //Check for the GET method, return 405 if not GET
    if (method != "GET") {
        std::string response = CreateHTTPResponse(HTTP_METHOD_NOT_ALLOWED);
        write(socket, response.c_str(), response.size());
        close(socket);
        return nullptr;
    }

    //If the requested file is '/', serve index.html
    if (requested_file == "/") {
        requested_file = "/index.html";
    }

    //Check for forbidden file request
    std::array forbidden_files{"passwords.txt", "secrets.txt"};
    for (const std::string& filename : forbidden_files) {
        if (requested_file.substr(1, requested_file.size()) == filename) {
            std::cerr << "SERVER: Forbidden file requested: " << requested_file
                      << "\n";
            std::string response = CreateHTTPResponse(HTTP_FORBIDDEN);
            write(socket, response.c_str(), response.size());
            close(socket);
            return nullptr;
        }
    }

    //Get the absolute path of the requested file
    std::array<char, BUFFER_SIZE> resolved_path =
        std::array<char, BUFFER_SIZE>();
    realpath((requested_file.c_str() + 1),
             resolved_path.data());  //Skip leading '/'

    //Get the current directory
    std::array<char, BUFFER_SIZE> current_dir = std::array<char, BUFFER_SIZE>();
    realpath(".", current_dir.data());

    //Check if the resolved path starts with the current directory
    if (strncmp(resolved_path.data(), current_dir.data(),
                strlen(current_dir.data())) != 0) {
        std::cerr << "SERVER: Forbidden: File outside current directory\n";
        const std::string response = CreateHTTPResponse(HTTP_FORBIDDEN);
        write(socket, response.c_str(), response.size());
        close(socket);
        return nullptr;
    }

    //Open and serve the file
    FILE* file = fopen(resolved_path.data(), "r");
    if (file == nullptr) {
        std::cerr << "SERVER: File not found\n";
        const std::string response = CreateHTTPResponse(HTTP_NOT_FOUND);
        write(socket, response.c_str(), response.size());
        close(socket);
        return nullptr;
    }

    std::string response = CreateHTTPResponse(HTTP_OKAY);
    write(socket, response.c_str(), response.size());

    while (fgets(buffer.data(), BUFFER_SIZE, file) != nullptr) {
        if (write(socket, buffer.data(), strlen(buffer.data())) == -1) {
            std::cerr << "SERVER: Error sending data\n";
            break;
        }
    }

    fclose(file);

    close(socket);
    return nullptr;
}

int main() {
    const std::string port = "80";

    //Signal handler for SIGINT
    struct sigaction sigint_handler;
    sigint_handler.sa_handler = handle_shutdown_signal;
    sigemptyset(&sigint_handler.sa_mask);
    sigint_handler.sa_flags = 0;
    sigaction(SIGINT, &sigint_handler, nullptr);
    sigaction(SIGTERM, &sigint_handler, nullptr);

    //Struct for getaddrinfo
    struct addrinfo hints {};
    struct addrinfo* servinfo{};
    struct addrinfo* p{};
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;      //Allow IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;  //TCP stream sockets
    hints.ai_flags = AI_PASSIVE;      //Use my IP

    //Uses getaddrinfo instead of depreciated gethostbyname
    if (getaddrinfo(nullptr, port.c_str(), &hints, &servinfo) != 0) {
        std::cerr << "SERVER: getaddrinfo error" << std::endl;
        return 1;
    }

    std::cout << "SERVER: Starting server on port: " << port << "\n";

    //Create a socket
    int server_sd = 0;
    for (p = servinfo; p != nullptr; p = p->ai_next) {
        server_sd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (server_sd == -1) {
            continue;  //Try the next one
        }

        const int on = 1;
        if (setsockopt(server_sd, SOL_SOCKET, SO_REUSEADDR, (char*)&on,
                       sizeof(on)) == -1) {
            std::cerr << "SERVER: setsockopt failed" << std::endl;
            close(server_sd);
            continue;  //Try the next one
        }

        //Bind the socket
        if (bind(server_sd, p->ai_addr, p->ai_addrlen) == -1) {
            close(server_sd);
            continue;  //Try the next one
        }
        break;  //Successfully bound
    }

    //Exit if failed to bind
    if (p == nullptr) {
        std::cerr << "SERVER: Failed to bind the socket" << "\n";
        //Free addrinfo structure as client_sd is connected
        freeaddrinfo(servinfo);
        return -1;
    }

    //Free addrinfo structure as client_sd is connected
    freeaddrinfo(servinfo);

    //Start listening
    if (listen(server_sd, CONNECTIONS) == -1) {
        std::cerr << "SERVER: Listen error" << "\n";
        close(server_sd);
        return -1;
    }

    //Accept connections until the process is killed
    while (server_running) {
        sockaddr_storage new_sock_addr{};
        socklen_t new_sock_addr_size = sizeof(new_sock_addr);

        //Accepting connections
        int* new_sd = new int;  //Dynamically allocate memory for the new socket
        *new_sd =
            accept(server_sd, (sockaddr*)&new_sock_addr, &new_sock_addr_size);
        if (*new_sd == -1) {
            if (!server_running) {
                delete new_sd;
                break;
            }
            std::cerr << "SERVER: Accept error" << "\n";
            delete new_sd;  //Clean up memory if accept fails
            continue;
        }

        std::cout << "SERVER: New connection\n";  //<< *new_sd << "\n";

        //Create the thread to run the create_client_thread function with the new_sd
        pthread_t thread = 0;
        if (pthread_create(&thread, nullptr, create_client_thread, new_sd) >
            0) {
            std::cerr << "SERVER: Thread not created. \n";
            close(*new_sd);
            delete new_sd;  //Clean up memory if thread creation fails
        }

        //Detach the thread to allow it to clean up after itself
        pthread_detach(thread);
    }

    close(server_sd);  //Close the server socket
    return 0;
}