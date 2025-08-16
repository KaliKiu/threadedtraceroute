
#include <fstream>
#include "../include/json.hpp"
#include "../include/packet_crafting.hpp"
#include "../include/utils.hpp"
#include <arpa/inet.h>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ifaddrs.h>
#include <unistd.h>
#include <netdb.h>
    //htons / ntohs required to change form lil endian to big endian

    Packet::ipheader* Packet::loadIPConfig(int ip){
        nlohmann::json json;
        std::ifstream file(Packet::IP_CONFIG_FILE);
        if(!Utils::checkIfFile(file)){
            std::cerr <<"checkfile";
            return 0;
        }
        
        file >>json;
        auto* p = new Packet::ipheader;
        uint8_t version = json["version"];
        uint8_t ihl = json["ihl"];
        version &= 0x0F;
        ihl &= 0xF;

        //8bit
        p->version_ihl = ((version<<4) | ihl);
        p->DSField_ECN = json["DSField_ECN"];
        //16bit
        p->total_length = htons(json["total_length"]);
        p->identification = htons(json["id"]);
        p->flags_fragoff = htons(json["flags_fragoff"]);
        //8bit
        p->ttl = json["ttl"];
        p->protocol = json["protocol"];
        //16bit
        p->headerChecksum = json["headerChecksum"]; //has to be calculated after
        //32bit (already network byte order)
        p->sourceIP = Packet::parseIPAddress(json["sourceIP"]); 
        p->destIP= Packet::parseIPAddress(json["destIP"][ip]);
        std::cout <<"MEOW" <<p->destIP;

        
        std::cout <<"IP hdr should be 20bytes, is: "<<sizeof(*p);
        
        return p;
    }
    Packet::icmpheader* Packet::loadICMPConfig(){
        nlohmann::json json;
        std::ifstream file(Packet::ICMP_CONFIG_FILE);
        if(!Utils::checkIfFile(file)){
            std::cout<<"meow";
            return 0;
        }
        
        file >>json;
        auto* p = new Packet::icmpheader;

        //8bit
        p->type = json["type"];
        p->code = json["code"];
        //16bit
        p->checksum = htons(json["checksum"]);
        p->id = htons(json["id"]);
        p->sequence = htons(json["sequence"]);
        
        std::cout<<"\nICMP hdr Size should be 8bytes, is: "<<sizeof(*p);
        return p;
    }
    uint16_t Packet::calculateIPChecksum(Packet::ipheader* hdr) {
    hdr->headerChecksum = 0; // zero checksum field

    uint16_t* data = reinterpret_cast<uint16_t*>(hdr);
    size_t length = sizeof(Packet::ipheader);

    uint32_t sum = 0;

    for (size_t i = 0; i < length / 2; ++i) {
        // Sum words as-is, no ntohs conversion
        sum += data[i];
        // Handle carry overflow
        if (sum > 0xFFFF)
            sum = (sum & 0xFFFF) + (sum >> 16);
    }

    uint16_t checksum = ~sum;
    return checksum;
    }
 
   uint16_t Packet::calculateICMPChecksum(Packet::icmpheader* hdr) {
    hdr->checksum = 0;
    uint16_t* data = reinterpret_cast<uint16_t*>(hdr);
    size_t length = sizeof(Packet::icmpheader);

    uint32_t sum = 0;
    for (size_t i = 0; i < length / 2; ++i) {
        sum += data[i];  // no ntohs here!
        if (sum > 0xFFFF)
            sum = (sum & 0xFFFF) + (sum >> 16);
    }
    return ~sum;
    }   
    //main FUNCTION
    std::vector<uint8_t> Packet::buildPackage(uint8_t ttl,int ip){
        std::vector<uint8_t> packet;

        //load config for ip and icmp
        Packet::ipheader* iphdr = Packet::loadIPConfig(ip);
        Packet::icmpheader* icmphdr = Packet::loadICMPConfig();
        iphdr->ttl=ttl;
        //calculate checksum for ip and icmp (no endian swap needed)
        iphdr->headerChecksum = Packet::calculateIPChecksum(iphdr);
        icmphdr->checksum = Packet::calculateICMPChecksum(icmphdr);
        printf("\nIP Checksum: dec: %d,hex:0x%x\nICMP Checksum: dec: %d,hex:0x%x",iphdr->headerChecksum,iphdr->headerChecksum,icmphdr->checksum,icmphdr->checksum);

        //store both hdr into uint8_t vector
        packet.insert(packet.end(),reinterpret_cast<uint8_t*>(iphdr),reinterpret_cast<uint8_t*>(iphdr)+sizeof(*iphdr));
        packet.insert(packet.end(),reinterpret_cast<uint8_t*>(icmphdr),reinterpret_cast<uint8_t*>(icmphdr)+sizeof(*icmphdr));

        //check if length matches
        if(sizeof(packet)==sizeof(iphdr)+sizeof(icmphdr)){
            std::cout <<"fatal size error";
            return {};
        }
        //reference the header struct pointers
        this->ippointer = iphdr;
        this->icmppointer = icmphdr;
        return packet;
    }

    void Packet::writePacketInFile(std::vector<uint8_t>& packet){
        std::ofstream file(Packet::PACKET_FILE,std::ios::binary);
        if(!file){
            std::cerr <<"couldnt open file";
            return;
        }
        file.write(reinterpret_cast<const char*>(packet.data()), packet.size());
        file.close();
    }

    uint32_t Packet::parseIPAddress(const std::string& ipStr) {
        struct in_addr addr;
        if (inet_pton(AF_INET, ipStr.c_str(), &addr) != 1) {
            std::cerr << "Invalid IP address format: " << ipStr << std::endl;
            return 0;  // or handle error as you like
        }
        return addr.s_addr;  // already in network byte order
    }
    