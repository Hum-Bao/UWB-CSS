#include "tcp.h"
#include <vector>

TCP::TCP(uint16_t source_port, uint16_t destination_port,
         uint32_t sequence_number, uint32_t ack_num,
         std::array<unsigned char, MAX_PAYLOAD> payload)
    : source_port_(source_port),
      destination_port_(destination_port),
      sequence_number_(sequence_number),
      ack_num_(ack_num),
      header_length_(std::bitset<4>(TCP::HEADER_LENGTH).to_ulong()),
      payload_(payload) {}

TCP::TCP() : TCP(0, 0, 0, 0, {}) {}

// Split data into single byte chunks and push into vector then copy vector to payload array
std::array<unsigned char, TCP::MAX_PAYLOAD + TCP::HEADER_LENGTH>
TCP::ToArray() {
    std::array<unsigned char, TCP::MAX_PAYLOAD + TCP::HEADER_LENGTH> payload{};
    std::vector<unsigned char> payload_vector;

    for (int count = 1; count >= 0; count--) {
        payload_vector.push_back((source_port_ >> (count * 8)) & 0xFF);
    }

    for (int count = 1; count >= 0; count--) {
        payload_vector.push_back((destination_port_ >> (count * 8)) & 0xFF);
    }

    for (int count = 3; count >= 0; count--) {
        payload_vector.push_back((sequence_number_ >> (count * 8)) & 0xFF);
    }

    for (int count = 3; count >= 0; count--) {
        payload_vector.push_back((ack_num_ >> (count * 8)) & 0xFF);
    }

    payload_vector.push_back((header_length_ & 0xF) << 4);

    payload_vector.push_back(flags_);

    for (int count = 1; count >= 0; count--) {
        payload_vector.push_back((window_ >> (count * 8)) & 0xFF);
    }

    for (int count = 1; count >= 0; count--) {
        payload_vector.push_back((checksum_ >> (count * 8)) & 0xFF);
    }

    for (int count = 1; count >= 0; count--) {
        payload_vector.push_back((urgent_pointer_ >> (count * 8)) & 0xFF);
    }

    payload_vector.insert(payload_vector.end(), payload_.begin(),
                          payload_.end());

    std::copy(payload_vector.begin(), payload_vector.end(), payload.begin());

    return payload;
}