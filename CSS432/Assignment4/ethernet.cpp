#include "ethernet.h"
#include <vector>
#include "functions.h"

Ethernet::Ethernet(const std::string& mac_destination,
                   const std::string& mac_source,
                   std::array<unsigned char, MAX_PAYLOAD> payload)
    : payload_(payload) {
    std::string dest = Functions::MACToBinary(mac_destination);
    std::string source = Functions::MACToBinary(mac_source);
    if (source.size() != 48 || dest.size() != 48) {
        std::string error = "Ethernet: ";
        if (source.size() != 48) {
            error +=
                ("Invalid source MAC length: " + std::to_string(source.size()));
        } else {
            error += ("Invalid destination MAC length: " +
                      std::to_string(dest.size()));
        }
        throw std::invalid_argument(error);
    }
    for (size_t count = 0; count < 6; ++count) {
        std::bitset<8> dest_byte(dest.substr(count * 8, 8));
        std::bitset<8> source_byte(source.substr(count * 8, 8));
        mac_destination_[count] = dest_byte.to_ulong();
        mac_source_[count] = source_byte.to_ulong();
    }
}

Ethernet::Ethernet() : Ethernet("00:00:00:00:00:00", "00:00:00:00:00:00", {}) {}

std::array<unsigned char, Ethernet::MAX_PAYLOAD + Ethernet::HEADER_LENGTH>
Ethernet::ToArray() {
    std::array<unsigned char, Ethernet::MAX_PAYLOAD + Ethernet::HEADER_LENGTH>
        payload{};
    std::vector<unsigned char> payload_vector;

    for (int count = 7; count >= 0; count--) {
        payload_vector.push_back((preamble_ >> (count * 8)) & 0xFF);
    }

    payload_vector.insert(payload_vector.end(), mac_destination_.begin(),
                          mac_destination_.end());

    payload_vector.insert(payload_vector.end(), mac_source_.begin(),
                          mac_source_.end());

    for (int count = 1; count >= 0; count--) {
        payload_vector.push_back((ethertype_ >> (count * 8)) & 0xFF);
    }

    payload_vector.insert(payload_vector.end(), payload_.begin(),
                          payload_.end());

    for (int count = 3; count >= 0; count--) {
        payload_vector.push_back((crc_ >> (count * 8)) & 0xFF);
    }

    std::copy(payload_vector.begin(), payload_vector.end(), payload.begin());

    return payload;
}

void Ethernet::Wrap(IPV6 datagram) {
    payload_ = datagram.ToArray();
}
