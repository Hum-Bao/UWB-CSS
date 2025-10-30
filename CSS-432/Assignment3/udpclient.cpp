#include <netdb.h>
#include <sys/time.h>
#include <unistd.h>
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

const int MESSAGE_LENGTH = 1460;
const int PACKET_COUNT = 20000;

/*
Case 1: Unreliable test simply sends 20,000 UDP packets from a client to a server. 
*/
void clientUnreliable(const int& sock, struct addrinfo* p,
                      std::array<int, MESSAGE_LENGTH>& message) {
    for (int count = 1; count <= PACKET_COUNT; count++) {
        //Set packet number
        message[0] = count;

        //Send packet
        if (sendto(sock, message.data(), message.size(), 0, p->ai_addr,
                   p->ai_addrlen) == -1) {
            std::cerr << "SERVER: Failed to send packet " << count << "\n";
        } else {
            //std::cout << "SERVER: PACKET: " << count << ", LENGTH: " << MESSAGE_LENGTH << " SENT\n";
        }
        //Delay as up 20% of packets were lost over localhost without it
        usleep(100);
    }
}

/*
Case 2: Stop-and-wait test implements the stop-and-wait algorithm (this can be done as a sliding window with a size = 1)
*/
int clientStopWait(const int& sock, struct addrinfo* p,
                   std::array<int, MESSAGE_LENGTH>& message) {
    message = {};

    //Set a timeout for receiving the acknowledgment
    struct timeval timeout {
            0, 50000
    };

    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) <
        0) {
        std::cerr << "Failed to set socket timeout\n";
        return -1;
    }

    std::array<int, MESSAGE_LENGTH> message_ack{};
    ssize_t bytes_received = 0;

    int retransmissions = 0;

    for (int count = 1; count <= PACKET_COUNT; count++) {
        //Set packet number
        message[0] = count;

        //Loop until packet is ACK'd
        while (true) {
            //Send message
            if (sendto(sock, message.data(), message.size(), 0, p->ai_addr,
                       p->ai_addrlen) == -1) {
                std::cerr << "CLIENT: Packet: " << count << "failed to send"
                          << "\n";
                continue;
            }

            //Try to receive ACK
            bytes_received =
                recvfrom(sock, message_ack.data(), message_ack.size(), 0,
                         p->ai_addr, &p->ai_addrlen);

            if (bytes_received > 0) {
                //Check if ACK matches packet num
                if (message_ack[0] == count) {
                    //std::cout << "CLIENT: ACK: " << count << "\n";
                    break;  //Exit loop
                }
            } else {
                //Resend on timeout
                std::cerr << "CLIENT: Timeout occurred for packet: " << count
                          << "\n";
            }
            retransmissions++;
        }
    }
    return retransmissions;  //Return the number of retransmissions
}

/*
Case 3: Sliding window implements a sliding window algorithm, using the first element in the message[] 
(a 1460 byte message, its contents don't matter other than the integer value in first element) for storing 
sequence numbers and acknowledgement numbers.
*/
int clientSlidingWindow(const int sock, struct addrinfo* p,
                        std::array<int, MESSAGE_LENGTH> message,
                        int windowSize) {
    message = {};

    //Set a timeout for receiving the acknowledgment
    struct timeval timeout {
            0, 50000
    };

    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) <
        0) {
        std::cerr << "Failed to set socket timeout\n";
        return -1;
    }

    ssize_t bytes_received = 0;
    int retransmissions = 0;
    int ack = 0;
    int window_start = 1;
    int next_seq_num = 1;
    int unacked_count = 0;  //Counter for unacknowledged messages

    while (window_start <= PACKET_COUNT) {
        //Send all packets in the window
        while (next_seq_num < window_start + windowSize &&
               next_seq_num <= PACKET_COUNT) {
            //Prepare the message to send
            message[0] = next_seq_num;

            //Send the packet
            if (sendto(sock, message.data(), message.size(), 0, p->ai_addr,
                       p->ai_addrlen) == -1) {
                std::cerr << "CLIENT: Packet " << next_seq_num
                          << " failed to send\n";
            } else {
                //std::cout << "CLIENT: Packet " << next_seq_num << " sent\n";
                unacked_count++;  //Increment unacknowledged messages
            }
            next_seq_num++;
        }

        //Try to receive an acknowledgment
        bytes_received =
            recvfrom(sock, &ack, sizeof(ack), 0, p->ai_addr, &p->ai_addrlen);

        if (bytes_received > 0) {
            //If an ACK is received, slide the window up to the received cumulative ACK
            //std::cout << "CLIENT: Received cumulative ACK " << ack << "\n";
            if (ack >= window_start) {
                //Decrement unacknowledged count
                unacked_count -= (ack - (window_start - 1));
                //Slide the window forward
                window_start = ack + 1;
            }
        } else {
            //Timeout occurred
            if (unacked_count >= windowSize) {
                std::cerr << "CLIENT: Timeout occurred, resending packets from "
                          << window_start << "\n";
                retransmissions++;
                //Reset the next sequence number to start resending
                next_seq_num = window_start;
            }
        }
    }

    return retransmissions;  //Return the number of retransmissions
}

//argv[1] = Test case #
//argv[2] = Server ip

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Invalid number of arguments. Current args: " << argc
                  << ". Expected args: " << 2 << "\n";
        return -1;
    }
    const int case_num = std::stoi(argv[1]);
    const std::string server_ip = argv[2];
    const std::string port = "2610";

    //Struct for getaddrinfo
    struct addrinfo hints {};
    struct addrinfo* servinfo{};
    struct addrinfo* p{};
    //Zero structure
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    //Uses getaddrinfo instead of deprecated gethostbyname
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
        break;  //Successfully connected
    }

    //Exit if failed to connect
    if (client_sd == -1) {
        std::cerr << "Failed to create socket\n";
        //Free addrinfo structure as socket is not created
        freeaddrinfo(servinfo);
        return -1;
    }
    freeaddrinfo(servinfo);

    std::array<int, MESSAGE_LENGTH> message{};

    switch (case_num) {
        case 1: {
            clientUnreliable(client_sd, p, message);
            break;
        }
        case 2: {
            int retransmissions = clientStopWait(client_sd, p, message);
            std::cout << "Retransmissions: " << retransmissions << "\n";
            break;
        }
        case 3: {
            int retransmissions =
                clientSlidingWindow(client_sd, p, message, 30);
            std::cout << "Retransmissions: " << retransmissions << "\n";
            break;
        }
        case 4: {
            struct timeval start {};

            struct timeval end {};

            gettimeofday(&start, nullptr);
            std::ofstream output_file("Client " + std::to_string(start.tv_sec) +
                                      ".txt");

            if (!output_file.is_open()) {
                std::cerr << "Error opening file!" << std::endl;
                return 1;  //Handle error
            }

            for (int window_size = 1; window_size <= 30; window_size++) {
                std::cout << "WINDOW: " << window_size << "\n";
                output_file << window_size << "\n";
                for (int test = 1; test <= 3; test++) {
                    gettimeofday(&start, nullptr);
                    message = {};
                    clientSlidingWindow(client_sd, p, message, window_size);
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

    close(client_sd);
}