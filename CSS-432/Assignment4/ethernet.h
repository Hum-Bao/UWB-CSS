#ifndef ETHERNET_H
#define ETHERNET_H

#include <array>
#include <cstdint>
#include <string>
#include "ipv6.h"

//Ethernet Header 1526 bytes, (1528 with padding)
class Ethernet {
    public:
        const static int MAX_PAYLOAD = 1500;
        const static int HEADER_LENGTH = 26;
        //Overloaded constructor
        Ethernet(const std::string& mac_destination,
                 const std::string& mac_source,
                 std::array<unsigned char, MAX_PAYLOAD> payload);

        //Default constructor
        Ethernet();

        //Pack all data in array to create payload
        std::array<unsigned char, MAX_PAYLOAD + HEADER_LENGTH> ToArray();

        void Wrap(IPV6 datagram);

    private:
        /*
        std::array<unsigned char, 8> preamble_ = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
                                                  0xAA, 0xAA, 0xAB};  // 8 bytes
                                                  */
        uint64_t preamble_ = 0xAAAAAAAAAAAAAAAB;          // 8 bytes
        std::array<unsigned char, 6> mac_destination_{};  // 6 bytes
        std::array<unsigned char, 6> mac_source_{};       // 6 bytes
        uint16_t ethertype_ = 0x80DD;                     // 2 bytes
        std::array<unsigned char, MAX_PAYLOAD> payload_;  // 1500 bytes
        uint32_t crc_ = 0xFFFFFFFF;                       // 4 bytes
};
#endif
