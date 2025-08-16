#ifndef PROBE
#define PROBE
namespace Probe{
#include <arpa/inet.h> 
#include <mutex>


void probe(uint8_t ttl,int ip,std::vector<std::string>& route,std::mutex& route_mutex);
void addIP(char ipStr[INET_ADDRSTRLEN],std::vector<std::string>& route,std::mutex& route_mutex);
}
#endif