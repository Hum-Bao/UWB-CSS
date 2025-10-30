#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <array>
#include <bitset>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

class Functions {
    public:
        static std::string ToBinary(const std::string& value);
        static std::string ToBinary(uint64_t value, uint8_t bit_count);

        static std::string ToDecimal(const std::string& value);

        static std::string RandomIPV6(const std::string& value);
        static std::string RandomMAC(const std::string& value);

        static std::string IPV6ToBinary(const std::string& value);
        static std::string BinaryToIPV6(const std::string& value);

        static std::string MACToBinary(const std::string& value);
        static std::string BinaryToMAC(const std::string& value);

        //Debug function
        template <size_t size>
        static void PrintPacketArray(
            const std::array<unsigned char, size>& packet) {
            for (int count = 0; count < packet.size(); count++) {
                std::cout << count << " ";
                if (count % 16 == 15) {
                    std::cout << std::bitset<8>(packet[count]) << "\n";
                } else {
                    std::cout << std::bitset<8>(packet[count]) << " ";
                }
            }
            std::cout << "\n";
        }

    private:
};
#endif