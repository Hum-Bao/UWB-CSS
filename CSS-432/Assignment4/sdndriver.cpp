#include <random>
#include "ethernet.h"
#include "functions.h"
#include "ipv6.h"
#include "sdnswitch.h"
#include "tcp.h"
#include "udp.h"

const int PACKET_NUM = 1000;

int main() {
    std::array<unsigned char, TCP::MAX_PAYLOAD> tcp_payload{};
    std::array<unsigned char, UDP::MAX_PAYLOAD> udp_payload{};
    tcp_payload.fill('T');
    udp_payload.fill('U');
    TCP tcp_packet;
    UDP udp_packet;
    IPV6 ipv6_datagram;
    Ethernet ethernet_frame;

    //Create RNG
    std::random_device rand;
    std::mt19937 gen(rand());
    std::uniform_int_distribution<> random_int(1, 3);
    std::uniform_int_distribution<> random_hop(1, 10);
    std::string source_ipv6;
    std::string dest_ipv6;
    int dest_port = 0;
    std::string dest_mac;
    int hop_limit = 0;
    for (int count = 0; count < PACKET_NUM; count++) {
        std::cout << "PACKET: " << count << "\n";
        hop_limit = random_hop(gen);
        switch (random_int(gen)) {
            //Firewall match
            case 1: {
                dest_ipv6 = "";
                dest_port = 0;
                std::uniform_int_distribution<> firewall(1, 2);
                //Match firewall routing rule
                if (firewall(gen) == 1) {
                    dest_ipv6 = Functions::RandomIPV6("fd00:");
                    dest_port = 25565;
                }
                //Match firewall blocking rule
                else {
                    if (firewall(gen) == 1) {
                        dest_ipv6 = Functions::RandomIPV6("fc00:");
                        dest_port = 80;
                    } else {
                        dest_ipv6 = Functions::RandomIPV6("ff00:");
                        dest_port = 22;
                    }
                }
                //TCP packet
                if (firewall(gen) == 1) {
                    tcp_packet = TCP(0, dest_port, count, count, tcp_payload);

                    ipv6_datagram = IPV6(hop_limit, Functions::RandomIPV6(""),
                                         dest_ipv6, tcp_packet.ToArray());
                    ethernet_frame = Ethernet(Functions::RandomMAC(""),
                                              Functions::RandomMAC(""),
                                              ipv6_datagram.ToArray());
                }
                //UDP packet
                else {
                    udp_packet = UDP(1, dest_port, udp_payload);
                    ipv6_datagram = IPV6(hop_limit, Functions::RandomIPV6(""),
                                         dest_ipv6, udp_packet.ToArray());
                    ethernet_frame = Ethernet(Functions::RandomMAC(""),
                                              Functions::RandomMAC(""),
                                              ipv6_datagram.ToArray());
                }
                SDNSwitch::MatchAction(ethernet_frame.ToArray());
                break;
            }
            //Router match
            case 2: {
                dest_ipv6 = "";
                std::uniform_int_distribution<> router(1, 2);
                //Match router forward rule
                if (router(gen) == 1) {
                    dest_ipv6 = Functions::RandomIPV6("fd00:");
                }
                //Match router block rule
                else {
                    if (router(gen) == 1) {
                        dest_ipv6 = Functions::RandomIPV6("fa00:");
                    } else {
                        dest_ipv6 =
                            "ff00:0000:0000:0000:0000:0000:0000:"
                            "0114";
                    }
                }
                //TCP packet
                if (router(gen) == 1) {
                    tcp_packet = TCP(0, 1, count, count, tcp_payload);

                    ipv6_datagram = IPV6(hop_limit, Functions::RandomIPV6(""),
                                         dest_ipv6, tcp_packet.ToArray());
                    ethernet_frame = Ethernet(Functions::RandomMAC(""),
                                              Functions::RandomMAC(""),
                                              ipv6_datagram.ToArray());
                }
                //UDP packet
                else {
                    udp_packet = UDP(2, 3, udp_payload);
                    ipv6_datagram = IPV6(hop_limit, Functions::RandomIPV6(""),
                                         dest_ipv6, udp_packet.ToArray());
                    ethernet_frame = Ethernet(Functions::RandomMAC(""),
                                              Functions::RandomMAC(""),
                                              ipv6_datagram.ToArray());
                }
                SDNSwitch::MatchAction(ethernet_frame.ToArray());
                break;
            }
            //NAT match
            case 3: {
                std::uniform_int_distribution<> nat(1, 3);
                std::uniform_int_distribution<> packet_type(1, 2);
                source_ipv6 = "";
                dest_ipv6 = "";
                int choice = nat(gen);
                //Both IPV6 addresses in private range, no NAT
                if (choice == 1) {
                    source_ipv6 = Functions::RandomIPV6("fc00:");
                    dest_ipv6 = Functions::RandomIPV6("fc00:");
                }
                //Destination in private range, no NAT
                else if (choice == 2) {
                    source_ipv6 = Functions::RandomIPV6("");
                    dest_ipv6 = Functions::RandomIPV6("fc00:");
                }
                //Destination in external range, NAT
                else {
                    source_ipv6 = Functions::RandomIPV6("fc00:");
                    dest_ipv6 = Functions::RandomIPV6("");

                    //TCP packet
                    if (packet_type(gen) == 1) {
                        tcp_packet = TCP(0, 1, count, count, tcp_payload);

                        ipv6_datagram = IPV6(hop_limit, source_ipv6, dest_ipv6,
                                             tcp_packet.ToArray());
                        ethernet_frame = Ethernet(Functions::RandomMAC(""),
                                                  Functions::RandomMAC(""),
                                                  ipv6_datagram.ToArray());
                    }
                    //UDP packet
                    else {
                        udp_packet = UDP(1, 2, udp_payload);
                        ipv6_datagram = IPV6(hop_limit, source_ipv6, dest_ipv6,
                                             udp_packet.ToArray());
                        ethernet_frame = Ethernet(Functions::RandomMAC(""),
                                                  Functions::RandomMAC(""),
                                                  ipv6_datagram.ToArray());
                    }
                    SDNSwitch::MatchAction(ethernet_frame.ToArray());
                    break;
                }
            }
            //Switch match
            case 4: {
                std::uniform_int_distribution<> switch_gen(1, 2);
                std::uniform_int_distribution<> mac_gen(0, 9);
                //Match switch allow rule
                if (switch_gen(gen) == 1) {
                    while (true) {
                        switch (mac_gen(gen)) {
                            case 0: {
                                dest_mac =
                                    Functions::RandomMAC("52:54:00:42:42:");
                                break;
                            }
                            case 2: {
                                dest_mac = Functions::RandomMAC("00:00:0C:");
                                break;
                            }
                            case 3: {
                                dest_mac = Functions::RandomMAC("98:CA:33:");
                                break;
                            }
                            case 4: {
                                dest_mac = Functions::RandomMAC("01:");
                                break;
                            }
                            case 5: {
                                dest_mac = Functions::RandomMAC("02:");
                                break;
                            }
                            case 6: {
                                dest_mac = Functions::RandomMAC("06:");
                                break;
                            }
                            case 7: {
                                dest_mac = Functions::RandomMAC("0A:");
                                break;
                            }
                            case 8: {
                                dest_mac = Functions::RandomMAC("0E:");
                                break;
                            }
                            default: {
                                continue;
                            }
                        }
                        break;
                    }
                }
                //Match switch block rule
                else {
                    if (switch_gen(gen) == 1) {
                        dest_mac = Functions::RandomMAC("70:B3:D5:E6:F0:");
                    } else {
                        dest_mac = Functions::RandomMAC("0F:");
                    }
                }
                //TCP packet
                if (mac_gen(gen) == 1) {
                    tcp_packet = TCP(0, 1, count, count, tcp_payload);

                    ipv6_datagram = IPV6(hop_limit, source_ipv6, dest_ipv6,
                                         tcp_packet.ToArray());
                    ethernet_frame =
                        Ethernet(dest_mac, Functions::RandomMAC(""),
                                 ipv6_datagram.ToArray());
                }
                //UDP packet
                else {
                    udp_packet = UDP(2, 3, udp_payload);
                    ipv6_datagram = IPV6(hop_limit, source_ipv6, dest_ipv6,
                                         udp_packet.ToArray());
                    ethernet_frame =
                        Ethernet(dest_mac, Functions::RandomMAC(""),
                                 ipv6_datagram.ToArray());
                }
                SDNSwitch::MatchAction(ethernet_frame.ToArray());
                break;
            }
        }
        std::cout << "\n";
    }
}
