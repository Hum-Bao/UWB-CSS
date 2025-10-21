#include "udp.h"
#include <vector>

UDP::UDP(uint16_t source_port, uint16_t destination_port,
         std::array<unsigned char, UDP::MAX_PAYLOAD> payload)
    : source_port_(source_port),
      destination_port_(destination_port),
      payload_(payload) {}

UDP::UDP() : UDP(0, 0, {}) {}

// Split data into single byte chunks and push into vector then copy vector to payload array
std::array<unsigned char, UDP::MAX_PAYLOAD + UDP::HEADER_LENGTH>
UDP::ToArray() {
    std::array<unsigned char, UDP::MAX_PAYLOAD + UDP::HEADER_LENGTH> payload{};
    std::vector<unsigned char> payload_vector;

    for (int count = 1; count >= 0; count--) {
        payload_vector.push_back((source_port_ >> (count * 8)) & 0xFF);
    }

    for (int count = 1; count >= 0; count--) {
        payload_vector.push_back((destination_port_ >> (count * 8)) & 0xFF);
    }

    for (int count = 1; count >= 0; count--) {
        payload_vector.push_back((length_ >> (count * 8)) & 0xFF);
    }

    for (int count = 1; count >= 0; count--) {
        payload_vector.push_back((checksum_ >> (count * 8)) & 0xFF);
    }

    payload_vector.insert(payload_vector.end(), payload_.begin(),
                          payload_.end());

    std::copy(payload_vector.begin(), payload_vector.end(), payload.begin());

    return payload;
}