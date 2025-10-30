#include "sdnswitch.h"
#include <array>
#include <iostream>
#include <string>
#include <utility>
#include "ethernet.h"
#include "functions.h"

const std::string& SDNSwitch::GetPublicIP() {
    const static std::string public_ip =
        "6b6a:55b5:14ad:a4c6:bcbe:8ee2:8cec:917c";
    return public_ip;
}

const std::vector<std::pair<std::pair<std::string, int>, int>>&
SDNSwitch::GetFirewallTable() {
    static const std::vector<std::pair<std::pair<std::string, int>, int>>
        firewall_table = {
            {{"fc00:0000:0000:0000:0000:0000:0000:0000/7", 80}, -1},
            {{"ff00:0000:0000:0000:0000:0000:0000:0000/8", 22}, -1},
            {{"fd00:0000:0000:0000:0000:0000:0000:0000/8", 25565}, 0}

            //{"ff00:0000:0000:0000:0000:0000:0000:0114/128",-1},  //Unicast experiments
            //{"fe80:0000:0000:0000/10", -1},  //Internal Link-Local
            // {"fd00:0000:0000:0000:0000:0000:0000:0000/8", 7},  //Internal Local Network
            // {"ff00:0000:0000:0000/8", 9},    //External Multicast
            // {"fc00:0000:0000:0000/7", 8},  //Internal Private
            // {"2000:0000:0000:0000/3", 6}    //External Unicast
        };
    return firewall_table;
}

const std::vector<std::pair<std::string, int>>& SDNSwitch::GetRoutingTable() {
    static const std::vector<std::pair<std::string, int>> routing_table = {
        {"ff00:0000:0000:0000:0000:0000:0000:0114/128",
         -1},  //Unicast experiments
        {"fa00:0000:0000:0000:0000:0000:0000:0000/8", -1},
        //{"fe80:0000:0000:0000/10", -1},  //Internal Link-Local
        {"fd00:0000:0000:0000:0000:0000:0000:0000/8",
         7},  //Internal Local Network
        // {"ff00:0000:0000:0000/8", 9},    //External Multicast
        // {"fc00:0000:0000:0000/7", 8},  //Internal Private
        // {"2000:0000:0000:0000/3", 6}    //External Unicast
    };

    return routing_table;
}

const std::vector<std::pair<std::string, int>>& SDNSwitch::GetNATTable() {
    static const std::vector<std::pair<std::string, int>> nat_table = {
        {"fe80:0000:0000:0000/10", -1},  //Internal Link-Local
        {"fd00:0000:0000:0000:0000:0000:0000:0000/8",
         -1},  //Internal Local Network
               //{"ff00:0000:0000:0000/8", 9},  //External Multicast
        {"fc00:0000:0000:0000/7", 0},  //Internal Private
        // {"2000:0000:0000:0000/3", 6}    //External Unicast
    };
    return nat_table;
}

const std::vector<std::pair<std::string, int>>& SDNSwitch::GetSwitchTable() {
    static const std::vector<std::pair<std::string, int>> switch_table = {
        {"52:54:00:42:42:00/40", 5},
        {"70:B3:D5:E6:F0:00/36", -1},
        {"00:00:0C:00:00:00/24", 2},
        {"98:CA:33:00:00:00/24", 3},
        {"01:00:00:00:00:00/8", 4},
        {"02:00:00:00:00:00/8", 1},
        {"06:00:00:00:00:00/8", 2},
        {"0A:00:00:00:00:00/8", 1},
        {"0E:00:00:00:00:00/8", 1},
        {"0F:00:00:00:00:00/8", -1}

    };
    return switch_table;
}

bool SDNSwitch::UpdateHopLimit(
    std::array<unsigned char, Ethernet::MAX_PAYLOAD + Ethernet::HEADER_LENGTH>
        frame) {
    frame[29]--;
    return frame[29] == 0;
}

void SDNSwitch::UpdateSourceIP(
    std::array<unsigned char, Ethernet::MAX_PAYLOAD + Ethernet::HEADER_LENGTH>&
        frame,
    const std::string& source_ip) {
    std::string temp = Functions::IPV6ToBinary(source_ip);

    int byte_index = 30;
    for (int i = 0; i < 16; ++i, ++byte_index) {
        std::string byte_str = temp.substr(i * 8, 8);
        frame[byte_index] =
            static_cast<unsigned char>(std::bitset<8>(byte_str).to_ulong());
    }
}

/*
Byte-array index:       Description:                    Value:
[00]-[07]               Ethernet preamble               = 0xAAAAAAAAAAAAAAAB;
[08]-[13]               Ethernet MAC destination
[14]-[19]               Ethernet MAC source
[20]-[21]               Ethernet ethertype              = 0x80DD;
[22]      >> 4          IPV6 version                    = 0x06
[22]      &  0x0F       IPV6 traffic class              = 0x0
[23]      >> 4          IPV6 traffic class              = 0x0
[23]      &  0x0F       IPV6 flow label                 = 0x0
[24]-[25]               IPV6 flow label                 = 0x0
[26]-[27]               IPV6 payload length             = 1460
[28]                    IPV6 next header                = 0x06/0x011
[29]                    IPV6 hop limit
[30]-[45]               IPV6 source address
[46]-[61]               IPV6 destination address
[62]-[63]               UDP/TCP source port
[64]-[65]               UDP/TCP destination port

//UDP
[66]-[67]               UDP packet length               = 1460
[68]-[69]               UDP CRC                         = 0xFFFF
[70]-[1521]             UDP payload

//TCP
[66]-[69]               TCP sequence number
[70]-[73]               TCP ACK number
[74]      >> 4          TCP header length               = 20
[74]      &  0x0F       Unused                          = 0x0
[75]                    TCP flags                       = 0x0
[76]-[77]               TCP window                      = 17520
[78]-[79]               TCP checksum                    = 0xFFFF
[79]-[80]               TCP urget pointer               = 0x0
[81]-[1521]             TCP payload

[1521]-[1525]           Ethernet CRC                   = 0xFFFFFFFF; 
*/
SDNSwitch::FrameInfo SDNSwitch::GetFrameInfo(
    std::array<unsigned char, Ethernet::MAX_PAYLOAD + Ethernet::HEADER_LENGTH>
        frame) {
    SDNSwitch::FrameInfo frameinfo{};

    for (int count = 8; count < 14; count++) {
        frameinfo.mac_destination += std::bitset<8>(frame[count]).to_string();
        frameinfo.mac_source += std::bitset<8>(frame[count + 6]).to_string();
    }

    frameinfo.hop_limit = frame[29];

    for (int count = 30; count < 46; count++) {
        frameinfo.ip_source += std::bitset<8>(frame[count]).to_string();
        frameinfo.ip_destination +=
            std::bitset<8>(frame[count + 16]).to_string();
    }

    for (int count = 62; count < 64; count++) {
        frameinfo.source_port += std::bitset<8>(frame[count]).to_string();
        frameinfo.destination_port +=
            std::bitset<8>(frame[count + 2]).to_string();
    }

    return frameinfo;
}

bool SDNSwitch::FirewallMatching(const struct SDNSwitch::FrameInfo& packet) {
    int range = 0;
    for (const std::pair<std::pair<std::string, int>, int>& pair :
         SDNSwitch::GetFirewallTable()) {
        range = stoi(pair.first.first.substr(pair.first.first.find('/') + 1));
        if (packet.ip_destination.substr(0, range) ==
                Functions::IPV6ToBinary(pair.first.first).substr(0, range) &&
            packet.destination_port ==
                Functions::ToBinary(pair.first.second, 16)) {

            if (pair.second == -1) {
                std::cout << "BLOCK: Packet destined for "
                          << Functions::BinaryToIPV6(packet.ip_destination)
                          << ":"
                          << Functions::ToDecimal(packet.destination_port)
                          << ", in forbidden range " << pair.first.first << ":"
                          << pair.first.second << "\n";
                return true;
            } else {
                std::cout << "ALLOW: Packet destined for "
                          << Functions::BinaryToIPV6(packet.ip_destination)
                          << ":"
                          << Functions::ToDecimal(packet.destination_port)
                          << ", matches rule " << pair.first.first << ":"
                          << pair.first.second << "\n";
                return false;
            }
        }
    }
    std::cout << "ALLOW: Packet destined for "
              << Functions::BinaryToIPV6(packet.ip_destination) << ":"
              << Functions::ToDecimal(packet.destination_port)
              << ", matches no rules\n";
    return false;
}

bool SDNSwitch::RouterMatching(const struct SDNSwitch::FrameInfo& packet) {
    int range = 0;
    for (const std::pair<std::string, int>& pair :
         SDNSwitch::GetRoutingTable()) {
        range = stoi(pair.first.substr(pair.first.find('/') + 1));
        if (packet.ip_destination.substr(0, range) ==
            Functions::IPV6ToBinary(pair.first).substr(0, range)) {
            if (pair.second == -1) {
                std::cout << "DROP: Packet destined for "
                          << Functions::BinaryToIPV6(packet.ip_destination)
                          << ", in forbidden range " << pair.first << "\n";
            } else {
                std::cout << "ROUTED: Packet destined for "
                          << Functions::BinaryToIPV6(packet.ip_destination)
                          << ", to port " << pair.second << "\n";
            }
            return true;
        }
    }
    std::cout << "ROUTED: Packet destined for "
              << Functions::BinaryToIPV6(packet.ip_destination)
              << " sent to NAT" << "\n";
    return false;
}

bool SDNSwitch::NATMatching(const struct SDNSwitch::FrameInfo& packet) {
    int range = 0;
    for (const std::pair<std::string, int>& pair : SDNSwitch::GetNATTable()) {
        range = stoi(pair.first.substr(pair.first.find('/') + 1));
        if (packet.ip_destination.substr(0, range) ==
            Functions::IPV6ToBinary(pair.first).substr(0, range)) {
            for (const std::pair<std::string, int>& source_pair :
                 SDNSwitch::GetNATTable()) {
                if (packet.ip_source.substr(0, range) ==
                    Functions::IPV6ToBinary(source_pair.first)
                        .substr(0, range)) {
                    std::cout << "NOT TRANSLATED: " << "Source IP: "
                              << Functions::BinaryToIPV6(packet.ip_source)
                              << " and destination IP: "
                              << Functions::BinaryToIPV6(packet.ip_destination)
                              << " in private range\n";
                    return false;
                }
            }
            std::cout << "NOT TRANSLATED: " << "Packet with source IP: "
                      << Functions::BinaryToIPV6(packet.ip_source)
                      << ", destination IP: "
                      << Functions::BinaryToIPV6(packet.ip_destination)
                      << " in private range\n";
            return false;
        }
    }
    std::cout << "TRANSLATED: Source packet address: "
              << Functions::BinaryToIPV6(packet.ip_source)
              << " to public address: " << GetPublicIP()
              << " and forwarded to external IPV6 address: "
              << Functions::BinaryToIPV6(packet.ip_destination) << "\n";
    return true;
}

bool SDNSwitch::SwitchMatching(const struct FrameInfo& packet) {
    int range = 0;
    for (const std::pair<std::string, int>& pair :
         SDNSwitch::GetSwitchTable()) {
        range = stoi(pair.first.substr(pair.first.find('/') + 1));
        if (packet.mac_destination.substr(0, range) ==
            Functions::MACToBinary(pair.first).substr(0, range)) {
            if (pair.second == -1) {
                std::cout << "DROPPED: Packet destined for "
                          << Functions::BinaryToMAC(packet.mac_destination)
                          << ", in forbidden range " << pair.first << "\n";
            } else {
                std::cout << "ROUTED: Packet destined for "
                          << Functions::BinaryToMAC(packet.mac_destination)
                          << " to port " << pair.second << "\n";
            }
            return true;
        }
    }
    std::cout << "DROPPED: Packet destined for "
              << Functions::BinaryToMAC(packet.mac_destination)
              << " does not match any NAT rules\n";
    return true;
}

void SDNSwitch::MatchAction(
    std::array<unsigned char, Ethernet::MAX_PAYLOAD + Ethernet::HEADER_LENGTH>
        frame) {

    bool drop = UpdateHopLimit(frame);

    FrameInfo packet = GetFrameInfo(frame);

    if (drop) {
        std::cout << "DROPPED: Hop limit of packet destined for "
                  << Functions::BinaryToIPV6(packet.ip_destination) << "is 0\n";
        return;
    }

    std::cout << "FIREWALL: " << "\n";
    if (FirewallMatching(packet)) {
        return;
    }

    std::cout << "ROUTER: " << "\n";
    if (RouterMatching(packet)) {
        return;
    }

    std::cout << "NAT: " << "\n";
    if (NATMatching(packet)) {
        UpdateSourceIP(frame, SDNSwitch::GetPublicIP());
        return;
    }

    std::cout << "SWITCH: " << "\n";
    if (SwitchMatching(packet)) {
        return;
    }
}
