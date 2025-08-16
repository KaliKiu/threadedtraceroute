#include "../include/packet_crafting.hpp"
#include "../include/transmission_socket.hpp"
#include "../include/utils.hpp"
#include "../include/json.hpp"
#include "../include/probe.hpp"
#include <mutex>
#include <iostream>
#include <stdio.h>
#include <arpa/inet.h> 
//Probe = single packet with specific ttl, same adrrIP
namespace Probe{
void probe(uint8_t ttl, int ip,std::vector<std::string>& route, std::mutex& route_mutex){
    Packet pack;
    Socket socket;
    std::vector<uint8_t> packet = pack.buildPackage(ttl,ip);
    printf("\nPackagebuilt, ttl: %d IP: %d",ttl,pack.ippointer->destIP);
    //send socket etc.
    Utils::printHexPacket(packet);
    socket.createSocket();
    socket.sendPacket(pack.ippointer,packet);
    printf("\nPacket sent");
    std::vector<uint8_t> rcvpacket = socket.rcvPacket(pack.ippointer,ttl,route,route_mutex);
    if(socket.quit==true){
        printf("\nNo message received");}
}
void addIP(char ipStr[INET_ADDRSTRLEN],std::vector<std::string>& route,std::mutex& route_mutex){
    std::lock_guard<std::mutex> guard(route_mutex);
    route.push_back(ipStr);
}
}