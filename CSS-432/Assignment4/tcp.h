#ifndef TCP_H
#define TCP_H

#include <array>
#include <bitset>
#include <cstdint>

//TCP header. 1460 bytes total
class TCP {
    public:
        //1440 is max size to fit into IPV6 Datagram
        static const int MAX_PAYLOAD = 1440;

        static const int HEADER_LENGTH = 20;

        //Overloaded constructor
        TCP(uint16_t source_port, uint16_t destination_port,
            uint32_t sequence_number, uint32_t ack_num,
            std::array<unsigned char, MAX_PAYLOAD> payload);

        //Default constructor
        TCP();

        //Pack all data in array to create payload
        std::array<unsigned char, MAX_PAYLOAD + HEADER_LENGTH> ToArray();

    private:
        uint16_t source_port_;           // 2 bytes
        uint16_t destination_port_;      // 2 bytes
        uint32_t sequence_number_;       // 4 bytes
        uint32_t ack_num_;               // 4 bytes
        uint8_t header_length_ : 4;      // 4 bits + 4 bits padding
        uint8_t flags_ = 0x0;            // 1 byte
        uint16_t window_ = 17520;        // 2 bytes
        uint16_t checksum_ = 0xFFFF;     // 2 bytes
        uint16_t urgent_pointer_ = 0x0;  // 2 bytes
        std::array<unsigned char, MAX_PAYLOAD> payload_{};  // 1440 bytes
};
#endif