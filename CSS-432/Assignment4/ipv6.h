#ifndef IPV6_H
#define IPV6_H

#include <array>
#include <cstdint>
#include <string>
#include "tcp.h"
#include "udp.h"

//IPV6 header. 1500 bytes total (1504 with alignment)
class IPV6 {

    public:
        const static int MAX_PAYLOAD = 1460;

        const static int HEADER_LENGTH = 40;

        //Overloaded constructor
        IPV6(uint8_t hop_limit, const std::string& source_address,
             const std::string& destination_address,
             std::array<unsigned char, MAX_PAYLOAD> payload);

        //Default constructor
        IPV6();

        // Wraps TCP packet in IPV6 packet (TCP packet -> IPV6 payload)
        void Wrap(TCP packet);

        // Wraps UDP packet in IPV6 packet (UDP packet -> IPV6 payload)
        void Wrap(UDP packet);

        // Pack all data in array to create payload
        std::array<unsigned char, MAX_PAYLOAD + HEADER_LENGTH> ToArray();

    private:
        uint8_t version_ : 4;                                  // 4 bits
        uint8_t traffic_class_ = 0x0;                          // 1 byte
        uint32_t flow_label_ : 20;                             // 20 bits
        uint16_t payload_length_ = MAX_PAYLOAD;                // 2 bytes
        uint8_t next_header_;                                  // 1 bytes
        uint8_t hop_limit_;                                    // 1 bytes
        std::array<unsigned char, 16> source_address_{};       // 16 bytes
        std::array<unsigned char, 16> destination_address_{};  // 16 bytes
        std::array<unsigned char, MAX_PAYLOAD> payload_{};     // 1460 bytes
};
#endif