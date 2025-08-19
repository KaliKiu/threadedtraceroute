
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
        std::vector<std::string> route;
        std::vector<std::thread> probes;
        std::mutex route_mutex; 

        for(uint8_t ttl = 1; ttl<100; ttl++){
            probes.emplace_back(Probe::probe,ttl,0,std::ref(route),std::ref(route_mutex));
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