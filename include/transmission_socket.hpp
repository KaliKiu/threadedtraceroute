#ifndef TRANSMISSION_SOCKET
#define TRANSMISSION_SOCKET

#include <sys/socket.h>
#include <vector>
#include <mutex>
#include "../include/packet_crafting.hpp"

class Socket{
public:
//socket file descriptors
static const int8_t MAX_NOREPLY_HOP = 10;
int32_t sendsocket_fd;
int32_t rcvsocket_fd;
bool quit;
void createSocket();
void sendPacket(Packet::ipheader* ippointer,std::vector<uint8_t> packet);
std::vector<uint8_t> rcvPacket(Packet::ipheader* ippointer,uint8_t& ttl,std::vector<std::string>& route, std::mutex& route_mutex);
};
#endif
