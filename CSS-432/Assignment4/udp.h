#ifndef UDP_H
#define UDP_H

#include <array>
#include <cstdint>

//UDP header. 1460 bytes total
class UDP {
    public:
        static const int MAX_PAYLOAD = 1452;
        static const int HEADER_LENGTH = 8;

        //Overloaded constructor
        UDP(uint16_t source_port, uint16_t destination_port,
            std::array<unsigned char, MAX_PAYLOAD> payload);

        // Default constructor
        UDP();

        //Pack all data in array to create payload
        std::array<unsigned char, MAX_PAYLOAD + HEADER_LENGTH> ToArray();

    private:
        uint16_t source_port_;                            // 2 bytes
        uint16_t destination_port_;                       // 2 bytes
        uint16_t length_ = MAX_PAYLOAD + HEADER_LENGTH;   // 2 bytes
        uint16_t checksum_ = 0xFFFF;                      // 2 bytes
        std::array<unsigned char, MAX_PAYLOAD> payload_;  // 1452 bytes
};
#endif