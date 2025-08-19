//create socket
#include <sys/socket.h>   // socket(), sendto(), recvfrom()
#include <netinet/in.h>   // sockaddr_in, IPPROTO_ICMP
#include <arpa/inet.h> 
#include <stdio.h>
#include <unistd.h>
#include "../include/packet_crafting.hpp"
#include "../include/transmission_socket.hpp"
#include "../include/probe.hpp"
#include <cstring>        
#include <iostream>
#include <vector>
#include <cerrno>
#include <mutex>
#include <string>

void Socket::createSocket(){
    //send socket
    int sendsocket_fd = socket(AF_INET, SOCK_RAW,IPPROTO_RAW);
    if(sendsocket_fd <0){perror("socket");return;}
    uint8_t on = 1;
    //option for costum built hdr
    if(setsockopt(sendsocket_fd, IPPROTO_IP,IP_HDRINCL,&on,sizeof(on))<0){
        perror("socketopt");return;}
    //rcv socket
    int rcvsocket_fd = socket(AF_INET,SOCK_RAW,IPPROTO_ICMP);
    this->sendsocket_fd = sendsocket_fd;
    this->rcvsocket_fd = rcvsocket_fd;

}
void Socket::sendPacket(Packet::ipheader* ippointer,std::vector<uint8_t> packet){
    if(this->sendsocket_fd<=0){
        perror("socket not created");return;}
    //fill socket address structure
    sockaddr_in sockaddr;
    memset(&sockaddr,0,sizeof(sockaddr));
    sockaddr.sin_family=AF_INET;
    sockaddr.sin_port=0;
    sockaddr.sin_addr.s_addr=ippointer->destIP;
    std::cout <<"\nIP:"<<sockaddr.sin_addr.s_addr<<"\n";
    //sendto function
    if(sendto(this->sendsocket_fd,packet.data(),packet.size(),0,(struct sockaddr*)&sockaddr,sizeof(sockaddr))<0){
        perror("sendto");
        close(this->sendsocket_fd);
        return;}
}
std::vector<uint8_t> Socket::rcvPacket(Packet::ipheader* ippointer,uint8_t& ttl,std::vector<std::string>& route, std::mutex& route_mutex){
    uint8_t Buffer[1024];
    sockaddr_in recvAddr;
    socklen_t addrLen = sizeof(recvAddr);
    //timeout stuff
    // Prepare fd_set for select
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(this->rcvsocket_fd, &readfds);

    // Set 5-second timeout
    struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    int ret = select(this->rcvsocket_fd+1,&readfds,nullptr,nullptr,&timeout);
     if (ret == -1) {
            perror("select");return {};
        } else if (ret == 0) {
            // Timeout, no data available
            printf("\nTimeout: no data received in %ldseconds\n",timeout.tv_sec);
            //quit waiting for probe response
            char ipStr[INET_ADDRSTRLEN] = "Error_hop";
            Probe::addIP(ipStr,route,route_mutex);
            this->quit=true;return {};
        }
    ssize_t recv_bytes= recvfrom(this->rcvsocket_fd,Buffer,sizeof(Buffer),0,
                        (struct sockaddr*)&recvAddr,&addrLen);
    if(recv_bytes<0){
        perror("recvfrom");close(this->rcvsocket_fd);return{};}
    char ipStr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET,&(recvAddr.sin_addr),ipStr,sizeof(ipStr));

    std::vector<uint8_t> packet(Buffer,Buffer+recv_bytes);
    printf("ROUTER %d,\nIP: ",ttl);std::cout<<ipStr<<"\n\n";
    Probe::addIP(ipStr,route,route_mutex);
    return packet;
}
