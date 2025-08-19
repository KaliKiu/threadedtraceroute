
#include "../include/packet_crafting.hpp"
#include "../include/transmission_socket.hpp"
#include "../include/utils.hpp"
#include "../include/json.hpp"
#include "../include/probe.hpp"
#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>

//
int main(){

    const uint32_t IP_ADDRESS_COUNT = 2;
    //per ip, 100 probes
    for(int i = 0; i=IP_ADDRESS_COUNT-1;i++){
        std::vector<std::string> route;
        std::vector<std::thread> probes;
        std::mutex route_mutex; 

        for(uint8_t ttl = 1; ttl<100; ttl++){
            probes.emplace_back(Probe::probe,ttl,i,std::ref(route),std::ref(route_mutex));
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        //wait for all threads to finish   
        for(auto &t : probes){
            t.join();
        }
        for(std::string r :route){
            std::cout<<"\n"<<r;
        }
        std::cout <<"\nAll joined";
    }
}


