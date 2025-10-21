#include "ipv6.h"
#include <cstdint>
#include <stdexcept>
#include "functions.h"

IPV6::IPV6(uint8_t hop_limit, const std::string& source_address,
           const std::string& destination_address,
           std::array<unsigned char, MAX_PAYLOAD> payload)
    : version_(0x6),
      flow_label_(0x0),
      hop_limit_(hop_limit),
      payload_(payload) {
    std::string source = Functions::IPV6ToBinary(source_address);
    std::string dest = Functions::IPV6ToBinary(destination_address);
    if (source.size() != 128 || dest.size() != 128) {
        std::string error = "IPV6: ";
        if (source.size() != 128) {
            error += ("Invalid source IPV6 length: " +
                      std::to_string(source.size()));
        } else {
            error += ("Invalid destination IPV6 length: " +
                      std::to_string(dest.size()));
        }
        throw std::invalid_argument(error);
        return;
    }
    for (size_t count = 0; count < 16; ++count) {
        // Take 8 bits from string
        std::bitset<8> source_byte(source.substr(count * 8, 8));
        std::bitset<8> dest_byte(dest.substr(count * 8, 8));
        // Convert to unsigned char
        source_address_[count] = source_byte.to_ulong();
        destination_address_[count] = dest_byte.to_ulong();
    }
}

IPV6::IPV6()
    : IPV6(0, "0000:0000:0000:0000:0000:0000:0000:0000",
           "0000:0000:0000:0000:0000:0000:0000:0000", {}) {}

void IPV6::Wrap(TCP packet) {
    next_header_ = 0x06;
    payload_ = packet.ToArray();
}

void IPV6::Wrap(UDP packet) {
    next_header_ = 0x11;
    payload_ = packet.ToArray();
}

std::array<unsigned char, IPV6::MAX_PAYLOAD + IPV6::HEADER_LENGTH>
IPV6::ToArray() {
    std::array<unsigned char, IPV6::MAX_PAYLOAD + IPV6::HEADER_LENGTH>
        payload{};
    std::vector<unsigned char> payload_vector;

    payload_vector.push_back(((version_ & 0xF) << 4) |
                             ((traffic_class_ >> 4) & 0xF));

    payload_vector.push_back(((traffic_class_ & 0xF) << 4) |
                             ((flow_label_ >> 16) & 0x0F));

    for (int count = 1; count >= 0; count--) {
        payload_vector.push_back(((flow_label_ >> (count * 8)) & 0xFF));
    }

    for (int count = 1; count >= 0; count--) {
        payload_vector.push_back(((payload_length_ >> (count * 8)) & 0xFF));
    }

    payload_vector.push_back(next_header_ & 0xFF);

    payload_vector.push_back(hop_limit_ & 0xFF);

    payload_vector.insert(payload_vector.end(), source_address_.begin(),
                          source_address_.end());

    payload_vector.insert(payload_vector.end(), destination_address_.begin(),
                          destination_address_.end());

    payload_vector.insert(payload_vector.end(), payload_.begin(),
                          payload_.end());

    std::copy(payload_vector.begin(), payload_vector.end(), payload.begin());

    return payload;
}
