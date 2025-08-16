#ifndef PACKET_CRAFTING
#define PACKET_CRAFTING
#include "../include/json.hpp"

class Packet{
    //packed struct
    public:
    
    typedef struct __attribute__((packed)){
        uint8_t version_ihl;
        uint8_t DSField_ECN;
        uint16_t total_length;
        uint16_t identification;
        uint16_t flags_fragoff;
        uint8_t ttl;
        uint8_t protocol;
        uint16_t headerChecksum;
        uint32_t sourceIP;
        uint32_t  destIP;
    } ipheader;

    typedef struct __attribute__((packed)){
        uint8_t type;
        uint8_t code;
        uint16_t checksum;
        uint16_t id;
        uint16_t sequence;
    }icmpheader;
    //getters and setters mby?
    static constexpr const char* IP_CONFIG_FILE = "../config/ip_config.json";
    static constexpr const char* ICMP_CONFIG_FILE = "../config/icmp_config.json";
    static constexpr const char* PACKET_FILE = "../data/packet.bin";
    ipheader* ippointer = new ipheader;
    icmpheader* icmppointer = new icmpheader;
    ipheader* loadIPConfig(int ip);
    icmpheader* loadICMPConfig();
    uint16_t calculateIPChecksum(ipheader* hdr);
    uint16_t calculateICMPChecksum(icmpheader* hdr);
    std::vector<uint8_t> buildPackage(uint8_t ttl,int ip);
    static uint32_t parseIPAddress(const std::string& ipSr);
    static void writePacketInFile(std::vector<uint8_t>& packet);
};
#endif