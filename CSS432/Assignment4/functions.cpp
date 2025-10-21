#include "functions.h"
#include <algorithm>
#include <bitset>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>

std::string Functions::ToBinary(const std::string& value) {
    std::string binary;
    for (int count = 0; count < value.length(); count++) {
        if (std::isdigit(value[count]) != 0) {
            binary += std::bitset<4>(value[count]).to_string();
        } else {
            binary += std::bitset<4>(value[count] + 9).to_string();
        }
    }
    return binary;
}

std::string Functions::ToDecimal(const std::string& value) {
    uint64_t decimal = 0;
    size_t len = value.length();

    // Check if binary string length is valid
    if (len == 0 || (len % 8 != 0)) {
        throw std::invalid_argument(
            "The binary string must represent valid binary data.");
    }

    for (size_t i = 0; i < len; ++i) {
        if (value[i] == '1') {
            decimal +=
                1LL << (len - 1 - i);  // Shift left by appropriate amount
        } else if (value[i] != '0') {
            throw std::invalid_argument(
                "The string must only contain '0' or '1'.");
        }
    }
    return std::to_string(decimal);
}

std::string Functions::ToBinary(uint64_t value, uint8_t bit_count) {
    switch (bit_count) {
        case 8:
            return std::bitset<8>(value).to_string();
        case 16:
            return std::bitset<16>(value).to_string();
        case 32:
            return std::bitset<32>(value).to_string();
        case 64:
            return std::bitset<64>(value).to_string();
        default:
            return "null";
    }
}

std::string Functions::RandomIPV6(const std::string& value) {
    std::vector<std::string> blocks(8, "0000");
    std::stringstream ss(value);
    std::string segment;
    int block_index = 0;

    while (std::getline(ss, segment, ':')) {
        if (!segment.empty()) {
            blocks[block_index++] = segment;
        } else {
            // Handle "::" for skipped zero blocks
            int skip_count = 8 - (blocks.size() - block_index);
            blocks.erase(blocks.begin() + block_index,
                         blocks.begin() + block_index + skip_count);
            block_index += skip_count;
        }
    }

    // Randomly fill remaining blocks
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 0xFFFF);

    for (int count = 0; count < 8; count++) {
        if (blocks[count] == "0000") {
            std::stringstream hex_stream;
            hex_stream << std::hex << std::setw(4) << std::setfill('0')
                       << dist(gen);
            blocks[count] = hex_stream.str();
        }
    }

    std::ostringstream result;
    for (size_t count = 0; count < blocks.size(); ++count) {
        if (count > 0) {
            result << ":";
        }
        result << blocks[count];
    }

    return result.str();
}

std::string Functions::RandomMAC(const std::string& value) {
    std::vector<std::string> segments(6, "XX");
    std::stringstream ss(value);
    std::string segment;
    int segment_index = 0;

    while (std::getline(ss, segment, ':')) {
        if (!segment.empty()) {
            segments[segment_index++] = segment;
        }
    }

    // Randomly fill remaining segments
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 0xFF);

    for (int i = 0; i < 6; ++i) {
        if (segments[i] == "XX") {
            std::stringstream hex_stream;
            hex_stream << std::hex << std::uppercase << std::setw(2)
                       << std::setfill('0') << dist(gen);
            segments[i] = hex_stream.str();
        }
    }

    std::ostringstream result;
    for (size_t i = 0; i < segments.size(); ++i) {
        if (i > 0) {
            result << ":";
        }
        result << segments[i];
    }

    return result.str();
}

std::string Functions::IPV6ToBinary(const std::string& value) {
    std::string temp = value;
    temp.erase(std::remove(temp.begin(), temp.end(), ':'), temp.end());
    return ToBinary(temp);
}

std::string Functions::BinaryToIPV6(const std::string& value) {
    if (value.size() != 128) {
        throw std::invalid_argument(
            "BinaryToIPV6(): Invalid binary length of: " +
            std::to_string(value.size()));
    }

    std::ostringstream ipv6_stream;
    ipv6_stream << std::hex << std::setfill('0');  // Set hex formatting

    for (size_t i = 0; i < 8; ++i) {
        // Extract 16-bit chunks
        std::string block_str = value.substr(i * 16, 16);
        // Convert binary string to int
        int block = std::stoi(block_str, nullptr, 2);
        // Format as 4 hex chars
        ipv6_stream << std::setw(4) << block;
        if (i < 7) {
            // Add colon separator
            ipv6_stream << ":";
        }
    }

    return ipv6_stream.str();
}

std::string Functions::MACToBinary(const std::string& value) {
    std::string temp = value;
    temp.erase(std::remove(temp.begin(), temp.end(), ':'), temp.end());
    return ToBinary(temp);
}

std::string Functions::BinaryToMAC(const std::string& value) {
    if (value.size() != 48) {
        throw std::invalid_argument(
            "The binary string must represent exactly 48 bits.");
    }

    std::ostringstream mac_stream;
    // Set hex formatting
    mac_stream << std::hex << std::setfill('0');

    for (size_t i = 0; i < 6; ++i) {
        // Extract 8-bit chunks
        std::string byte_str = value.substr(i * 8, 8);
        // Convert binary string to int
        int byte = std::stoi(byte_str, nullptr, 2);
        // Format as 2 hex chars
        mac_stream << std::setw(2) << byte;
        if (i < 5) {
            // Add colon separator
            mac_stream << ":";
        }
    }

    return mac_stream.str();
}