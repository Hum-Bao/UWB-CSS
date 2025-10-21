#include <netdb.h>
#include <sys/time.h>
#include <unistd.h>
#include <array>
#include <atomic>
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <random>
#include <set>
#include <string>

//Atomic bool for allowing server to gracefully close on CTRL + C
std::atomic<bool> server_running(true);

const int MESSAGE_LENGTH = 1460;
const int PACKET_COUNT = 20000;

//Signal handler to catch SIGINT (CTRL + C)
void handle_shutdown_signal(int signal) {
    if (signal == SIGINT || signal == SIGTERM) {
        std::cout << "SERVER: Shutdown signal received. Stopping server\n";
        server_running = false;
    }
}

/*
Case 1: Receives message[] from a client using the sock object 20,000 times. 
Does not send back any acknowledgment to the client.
*/
void serverUnreliable(const int& sock, struct addrinfo* p,
                      std::array<int, MESSAGE_LENGTH>& message) {

    //Set socket timeout to 5 seconds to avoid server hanging
    struct timeval timeout {
            5, 0
    };

    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) <
        0) {
        std::cerr << "SERVER: Set SO_RCVTIMEO failed\n";
        close(sock);
        return;
    }

    //Zero message array
    message = {};

    std::set<int> received_packets;
    ssize_t bytes_received = 0;
    int current_packet_num = 0;

    while (server_running && current_packet_num < PACKET_COUNT) {
        bytes_received = recvfrom(sock, message.data(), sizeof(message), 0,
                                  p->ai_addr, &p->ai_addrlen);
        if (bytes_received < 0) {
            //Exit only on timeout, not on other wait
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                std::cerr
                    << "SERVER: Exiting. No packets received for 5 seconds.\n";
                break;
            } else {
                std::cerr << "SERVER: ERROR: No data received\n";
                continue;
            }
        }

        if (received_packets.insert(message[0]).second) {
            current_packet_num++;
            //     std::cout << "Packet " << message[0] << " received in order.\n";
        } else {
            std::cout << "Duplicate packet received: " << message[0] << "\n";
        }
    }

    int missing = 0;
    for (int count = 1; count <= PACKET_COUNT; count++) {
        if (received_packets.find(count) == received_packets.end()) {
            std::cout << "SERVER: Missing packet: " << count << "\n";
            missing++;
        }
    }
    if (missing == 0) {
        std::cout << "SERVER: All packets received\n";
    } else {
        std::cout << missing << "/" << PACKET_COUNT << " or "
                  << (static_cast<double>(missing) / PACKET_COUNT) * 100
                  << "% lost.\n";
    }

    close(sock);
}

/*
Case 2: Stop-and-wait test implements the stop-and-wait algorithm. Server repeats 
receiving message[] and sending an acknowledgment 20,000 times using the sock object.  
Packets are randomly dropped 1% of the time.
*/
void serverReliable(const int& sock, struct addrinfo* p,
                    std::array<int, MESSAGE_LENGTH>& message) {
    //Zero message array
    message = {};

    //Create RNG
    std::random_device rand;
    std::mt19937 gen(rand());
    std::uniform_int_distribution<> random_int(1, 100);

    std::array<int, MESSAGE_LENGTH> message_ack{};

    ssize_t bytes_received = 0;
    int total_received = 0;
    int expected_packet_num = 1;
    int dropped = 0;

    while (server_running && total_received < PACKET_COUNT) {
        //Receive message
        bytes_received = recvfrom(sock, message.data(), message.size(), 0,
                                  p->ai_addr, &p->ai_addrlen);

        if (bytes_received > 0) {
            //Simulate a 1% packet drop
            if (random_int(gen) == 1) {
                std::cout << "SERVER: Packet: " << message[0] << " dropped\n";
                dropped++;
                continue;
            }
            // Only process expected packet
            if (message[0] == expected_packet_num) {
                // Prepare ACK
                message_ack[0] = message[0];

                if (sendto(sock, message_ack.data(), message_ack.size(), 0,
                           p->ai_addr, p->ai_addrlen) == -1) {
                    std::cerr
                        << "SERVER: Failed to ACK packet: " << message_ack[0]
                        << "\n";
                    dropped++;
                } else {
                    //   std::cout << "SERVER: Packet: " << message_ack[0] << " ACK'd\n";
                    total_received++;
                    // Move to the next expected packet
                    expected_packet_num++;
                }
            } else {
                std::cout << "SERVER: Out of order packet received: "
                          << message[0] << "\n";
            }
        } else {
            std::cerr << "SERVER: Failed to receive packet\n";
        }
    }

    (dropped == 0)
        ? std::cout << "SERVER: All packets received\n"
        : std::cout << "SERVER: " << dropped << "/" << PACKET_COUNT << " or "
                    << (static_cast<double>(dropped) / PACKET_COUNT) * 100
                    << "% lost.\n";
}

/*
Case 3: Implements a sliding window algorithm, using the first element in the message[] for 
storing sequence numbers and acknowledgement numbers. Receives message[] and sends an 
acknowledgment to the client 20,000 times. Every time the server receives a new message[], 
it saves the message's sequence number in its array and return an acknowledgement (as described above).
Packets are randomly dropped 1% of the time.
*/
void serverEarlyRetrans(const int& sock, struct addrinfo* p,
                        std::array<int, MESSAGE_LENGTH>& message,
                        int windowSize) {
    //Zero message array
    message = {};

    //Create RNG
    std::random_device rand;
    std::mt19937 gen(rand());
    std::uniform_int_distribution<> random_int(1, 100);

    //Track received packets in current window
    std::vector<bool> received_packets(windowSize, false);

    //Number of bytes received from client per packet
    ssize_t bytes_received = 0;

    //Number of dropped packets
    int dropped = 0;

    //Total received packets
    int total_received = 0;

    //Highest packet number acknowledged in order
    int cumulative_ack = 1;

    //Starting packet num
    int current_packet_num = 1;

    //Latest received packet num
    int received_packet_num = 0;

    while (server_running && total_received < PACKET_COUNT) {
        //Receive message
        bytes_received = recvfrom(sock, message.data(), message.size(), 0,
                                  p->ai_addr, &p->ai_addrlen);

        if (bytes_received > 0) {
            received_packet_num = message[0];

            //Check if packet is in the current window
            if (received_packet_num >= current_packet_num &&
                received_packet_num < current_packet_num + windowSize) {
                //Simulate a 1% packet drop
                if (random_int(gen) == 1) {
                    std::cout << "SERVER: Packet " << received_packet_num
                              << " dropped\n";
                    dropped++;
                    continue;
                }

                //Mark packet as received
                received_packets[received_packet_num - current_packet_num] =
                    true;

                //Update cumulative acknowledgment
                while (!received_packets.empty() && received_packets.front()) {
                    //Remove ackd packets from vector
                    received_packets.erase(received_packets.begin());
                    received_packets.push_back(false);
                    cumulative_ack = current_packet_num;
                    current_packet_num++;
                    total_received++;
                }

                //Send cumulative ACK
                if (sendto(sock, &cumulative_ack, sizeof(cumulative_ack), 0,
                           p->ai_addr, p->ai_addrlen) == -1) {
                    std::cerr
                        << "SERVER: Failed to send cumulative ACK for packet "
                        << cumulative_ack << "\n";
                    dropped++;
                    continue;
                } else {
                    //std::cout << "SERVER: Cumulative ACK " << cumulative_ack << " sent\n";
                }
            } else {
                //std::cout << "SERVER: Packet " << received_packet_num<< " out of window range\n";
            }
        } else {
            std::cerr << "SERVER: Failed to receive packet\n";
        }
    }

    (dropped == 0)
        ? std::cout << "SERVER: All packets received\n"
        : std::cout << "SERVER: " << dropped << "/" << PACKET_COUNT << " or "
                    << (static_cast<double>(dropped) / PACKET_COUNT) * 100
                    << "% lost.\n";
}

//argv[1] = Test case #
int main(int argc, char* argv[]) {

    if (argc != 2) {
        std::cerr << "Invalid number of arguments. Current args: " << argc
                  << ". Expected args: " << 1 << "\n";
    }

    int case_num = std::stoi(argv[1]);

    //Signal handler for SIGINT
    struct sigaction sigint_handler {};

    sigint_handler.sa_handler = handle_shutdown_signal;
    sigemptyset(&sigint_handler.sa_mask);
    sigint_handler.sa_flags = 0;
    sigaction(SIGINT, &sigint_handler, nullptr);
    sigaction(SIGTERM, &sigint_handler, nullptr);

    const std::string port = "2610";

    //Struct for getaddrinfo
    struct addrinfo hints {};
    struct addrinfo* servinfo{};
    struct addrinfo* p{};
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;     //Allow IPv4 or IPv6
    hints.ai_socktype = SOCK_DGRAM;  //UDP
    hints.ai_flags = AI_PASSIVE;     //Use my IP

    //Uses getaddrinfo instead of deprecated gethostbyname
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
        if (setsockopt(server_sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) <
            0) {
            std::cerr << "SERVER: Set SO_REUSEADDR failed\n";
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
        //Free addrinfo structure as socket is not created
        freeaddrinfo(servinfo);
        return -1;
    }

    //Free addrinfo structure
    freeaddrinfo(servinfo);

    std::array<int, MESSAGE_LENGTH> message{};
    switch (case_num) {
        case 1: {
            serverUnreliable(server_sd, p, message);
            break;
        }
        case 2: {
            serverReliable(server_sd, p, message);
            break;
        }
        case 3: {
            serverEarlyRetrans(server_sd, p, message, 30);
            break;
        }
        //Benchmark
        case 4: {
            struct timeval start {};

            struct timeval end {};

            gettimeofday(&start, nullptr);
            std::ofstream output_file("Server " + std::to_string(start.tv_sec) +
                                      ".txt");

            if (!output_file.is_open()) {
                std::cerr << "Error opening file!" << std::endl;
                return 1;  // Handle error
            }

            for (int window_size = 1; window_size <= 30; window_size++) {
                std::cout << "WINDOW: " << window_size << "\n";
                output_file << window_size << "\n";
                for (int test = 1; test <= 3; test++) {
                    gettimeofday(&start, nullptr);
                    message = {};
                    serverEarlyRetrans(server_sd, p, message, window_size);
                    gettimeofday(&end, nullptr);
                    uint64_t time = (end.tv_sec - start.tv_sec) * 1000000 +
                                    (end.tv_usec - start.tv_usec);
                    output_file << time << "\n";
                    usleep(1000000);
                }
            }
            output_file.close();
            break;
        }
        default:
            std::cout << "SERVER: Unknown test case: " << case_num << "\n";
            return -1;
    }

    close(server_sd);
    return 0;
}