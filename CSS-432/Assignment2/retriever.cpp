#include <bits/types/struct_iovec.h>
#include <bits/types/struct_timeval.h>
#include <netdb.h>       //gethostbyname
#include <sys/socket.h>  //socket, bind, listen, inet_ntoa
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/uio.h>  //writev
#include <unistd.h>   //read, write, close
#include <array>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

const int BUFFER_SIZE = 1024;

//Generate unique filename
std::string CreateFilename(const std::string& file_name) {
    //Fix for unclear file names
    std::string unique_file_name = file_name;
    if (unique_file_name == "") {
        unique_file_name = "index";
    }

    std::string base_file;
    std::size_t dot_pos = unique_file_name.find('.');
    std::string extension;

    extension = (dot_pos != std::string::npos)
                    ? unique_file_name.substr(dot_pos)
                    : ".html";  //Default extension if no dot is found

    if (dot_pos == std::string::npos) {
        unique_file_name += extension;
    }

    int version = 0;

    struct stat buf {};

    //Check for existing filename, and generate new filename with (version) appended.
    //Checks for existing (version)s and edits the string so we get (2) instead of (1)(2)
    while (stat(unique_file_name.c_str(), &buf) != -1) {
        std::string base_file =
            unique_file_name.substr(0, unique_file_name.find('.'));
        if (base_file.find_last_of(')') != std::string::npos) {
            if (isdigit(base_file.at(base_file.find_last_of(')') - 1)) != 0) {
                int current_version =
                    std::stoi(base_file.substr(base_file.find_last_of(')') - 1,
                                               base_file.find_last_of(')')));
                version = current_version + 1;
                unique_file_name =
                    base_file.substr(0, unique_file_name.find_last_of(')') - 1);
                unique_file_name += std::to_string(version) + ")" + extension;
            }
        } else {
            unique_file_name =
                (base_file += '(' + std::to_string(1) + ')' + extension);
        }
    }

    return unique_file_name;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "RETRIEVER: Please specify URL for GET request\n";
        return -1;
    }
    std::string special_request = (argv[2] != nullptr) ? argv[2] : "";
    bool unallowed = false;
    bool malformed = false;

    if (special_request == "unallowed") {
        std::cout << "RETRIEVER: Unallowed test\n";
        unallowed = true;
    } else if (special_request == "malformed") {
        std::cout << "RETRIEVER: Malformed test\n";
        malformed = true;
    }

    std::string url = argv[1];

    //Strip HTTP/HTTPS prefix from URL
    size_t prefix_location = url.find("://");
    url = (prefix_location != std::string::npos)
              ? url = url.substr(prefix_location + 3)
              : url;

    std::size_t slash_pos = url.find('/');

    //Get server ip without file path
    std::string server_ip =
        (slash_pos != std::string::npos) ? url.substr(0, slash_pos) : url;
    //Get filepath
    std::string file_path =
        (slash_pos != std::string::npos) ? url.substr(slash_pos) : "/";

    const std::string port = "80";

    //Struct for getaddrinfo
    struct addrinfo hints {};
    struct addrinfo* servinfo{};
    struct addrinfo* p{};
    //Zero structure
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    //Uses getaddrinfo instead of depreciated gethostbyname
    const int status =
        getaddrinfo(server_ip.c_str(), port.c_str(), &hints, &servinfo);
    if (status != 0) {
        perror("getaddrinfo");
        std::cerr << "getaddrinfo error\n";
        return -1;
    }

    int client_sd = -1;
    for (p = servinfo; p != nullptr; p = p->ai_next) {
        client_sd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (client_sd == -1) {
            continue;  //Try next if socket creation fails
        }

        if (connect(client_sd, p->ai_addr, p->ai_addrlen) == -1) {
            close(client_sd);
            continue;  //Try next if connection fails
        }
        break;  //Successfully connected
    }

    //Exit if failed to connect
    if (p == nullptr) {
        std::cerr << "Failed to connect to the server\n";
        //Free addrinfo structure as client_sd is connected
        freeaddrinfo(servinfo);
        return -1;
    }

    //Free addrinfo structure as client_sd is connected
    freeaddrinfo(servinfo);

    std::string request;

    if (unallowed) {
        //Create unallowed HTTP POST request
        request = "POST " + file_path + " HTTP/1.1\r\n";
        request += "Host: " + std::string(server_ip) + "\r\n";
        request += "Connection: close\r\n";
        request += "\r\n";  //End of request header
    } else if (malformed) {
        //Create malformed HTTP GET request
        request = "GeT" + file_path + "HTtP/1.1";
        request += "Host:" + std::string(server_ip) + "";
        request += "Connection:close\r";
        request += "\n";  //End of request header
    } else {
        //Create HTTP GET request
        request = "GET " + file_path + " HTTP/1.1\r\n";
        request += "Host: " + std::string(server_ip) + "\r\n";
        request += "Connection: close\r\n";
        request += "\r\n";  //End of request header
    }

    //Send HTTP GET request to server
    ssize_t bytes_sent = send(client_sd, request.c_str(), request.size(), 0);
    if (bytes_sent == -1) {
        std::cerr << "Error sending request\n";
        close(client_sd);
        return -1;
    }

    //Read and save response from server
    std::array<char, BUFFER_SIZE> buffer = std::array<char, BUFFER_SIZE>();
    ssize_t bytes_received = 0;
    bool is_header = true;
    std::string response_data;

    // Read response from server
    while ((bytes_received =
                read(client_sd, buffer.data(), buffer.size() - 1)) > 0) {
        buffer[bytes_received] = '\0';        // Null-terminate response
        response_data.append(buffer.data());  // Add response data

        if (is_header) {
            // Check if header has been read
            std::size_t header_end = response_data.find("\r\n\r\n");
            if (header_end != std::string::npos) {
                is_header = false;

                // Parse headers
                std::string headers = response_data.substr(0, header_end);
                response_data =
                    response_data.substr(header_end + 4);  // Move past headers
            }
        }
    }

    if (bytes_received == 0) {
        // std::cerr << "No response from server\n";
    } else if (bytes_received == -1) {
        std::cerr << "Error reading from server\n";
    }

    //Get filename
    std::string file_name =
        CreateFilename(file_path.substr(file_path.find_last_of('/') + 1));

    //Open file to save response
    std::ofstream outfile(file_name);
    if (!outfile.is_open()) {
        std::cerr << "Error opening file for writing\n";
        close(client_sd);
        return -1;
    }

    // Write the body to the file
    outfile << response_data;  // Write the accumulated response data

    //Close the file and the client socket
    outfile.close();
    close(client_sd);

    std::cout << "RETRIEVER: Response saved to: " << file_name << "\n";
    return 0;
}
