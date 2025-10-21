#include <bits/types/struct_iovec.h>
#include <bits/types/struct_timeval.h>
#include <netdb.h>       // gethostbyname
#include <sys/socket.h>  // socket, bind, listen, inet_ntoa
#include <sys/time.h>
#include <sys/uio.h>  // writev
#include <unistd.h>   // read, write, close
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

const int NUM_ARGS = 7;
const int PORT_OFFSET = 1024;
const int BUFFER_SIZE = 1500;

/*
Your client program must receive the following six arguments:

port: a server IP port
repetition: the repetition of sending a set of data buffers
nbufs: the number of data buffers
bufsize: the size of each data buffer (in bytes)
serverIp: a server IP name
type: the type of transfer scenario: 1, 2, or 3
*/
int main(int argc, char* argv[]) {
    if (argc != NUM_ARGS) {
        std::cout << "Need 6 args, currently have: " << argc - 1 << "\n";
        for (int count = 0; count < argc; count++) {
            std::cout << argv[count] << "\n";
        }
        return -1;
    }

    std::string port = argv[1];
    if (stoi(port) < PORT_OFFSET) {
        port = std::to_string(stoi(port) + PORT_OFFSET);
    }

    const int repetitions = std::stoi(argv[2]);
    const int nbufs = std::stoi(argv[3]);
    const int bufsize = std::stoi(argv[4]);

    if (nbufs * bufsize != BUFFER_SIZE) {
        std::cerr << "nbufs * bufsize must equal 1500. Curently equals: "
                  << nbufs * bufsize << "\n";
        return -1;
    }

    const char* server_ip = argv[5];
    const int type = std::stoi(argv[6]);

    //Struct for getaddrinfo
    struct addrinfo hints {};
    struct addrinfo* servinfo{};
    struct addrinfo* p{};
    //Zero structure
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    //Uses getaddrinfo instead of depreciated gethostbyname
    const int status = getaddrinfo(server_ip, port.c_str(), &hints, &servinfo);
    if (status != 0) {
        std::cerr << "getaddrinfo error\n";
        return -1;
    }

    int client_sd = -1;
    for (p = servinfo; p != nullptr; p = p->ai_next) {
        client_sd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (client_sd == -1) {
            continue;  // Try next if socket creation fails
        }

        if (connect(client_sd, p->ai_addr, p->ai_addrlen) == -1) {
            close(client_sd);
            continue;  // Try next if connection fails
        }
        break;  // Successfully connected
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

    std::vector<std::vector<char>> databuf(nbufs, std::vector<char>(bufsize));

    // Fill with 'X' for testing
    for (int count = 0; count < nbufs; count++) {
        std::fill(databuf[count].begin(), databuf[count].end(), 'X');
    }

    struct timeval start {};

    struct timeval lap {};

    struct timeval end {};

    //Start
    gettimeofday(&start, nullptr);

    int iterations = 0;
    while (iterations < repetitions) {
        switch (type) {
            case 1: {
                // Send using multiple write() calls
                for (int count = 0; count < nbufs; count++) {
                    if (write(client_sd, databuf[count].data(), bufsize) ==
                        -1) {
                        std::cerr << "write error: " << strerror(errno) << "\n";
                        close(client_sd);
                        return -1;
                    }
                }
                break;
            }
            case 2: {
                // Send using writev()
                std::vector<struct iovec> vector(nbufs);
                for (int count = 0; count < nbufs; count++) {
                    vector[count].iov_base = databuf[count].data();
                    vector[count].iov_len = bufsize;
                }
                if (writev(client_sd, vector.data(), nbufs) == -1) {
                    std::cerr << "writev error: " << strerror(errno) << "\n";
                    close(client_sd);
                    return -1;
                }
                break;
            }

            case 3: {
                // Send all data in one go using a single write() call
                if (write(client_sd, databuf[0].data(), BUFFER_SIZE) == -1) {
                    std::cerr << "write error: " << strerror(errno) << "\n";
                    close(client_sd);
                    return -1;
                }
                break;
            }

            default: {
                std::cerr << "Invalid write type: " << type << "\n";
                return -1;
            }
        }
        iterations++;
    }

    //Lap time
    gettimeofday(&lap, nullptr);

    const long send_time =
        (lap.tv_sec - start.tv_sec) * 1000000 + (lap.tv_usec - start.tv_usec);

    int read_num = 0;

    while (true) {
        //Get response from server
        int num_bytes = read(client_sd, &read_num, sizeof(read_num));

        if (num_bytes == -1) {
            std::cerr << "Error reading from server\n";
            return -1;
        } else if (num_bytes == 0) {
            //Successful read
            break;
        }
    }

    //Get timestamp for end of server-client communications
    gettimeofday(&end, nullptr);

    //Round-trip-time calculation
    long rtt =
        (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

    std::cout << "Test " << type << ": data-sending time = " << send_time
              << " usec, round-trip time = " << rtt
              << " usec, #read = " << read_num << std::endl;

    //Close client and exit program
    close(client_sd);
    return 0;
}
