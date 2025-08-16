#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "../include/packet_crafting.hpp"

namespace Utils{
    bool checkIfFile(std::ifstream& file){
        if(file.is_open()) {
            if (file.peek() == std::ifstream::traits_type::eof()) {
                std::cout << "file empty.\n";
                return false;
            }
        }else {
            std::cout << "failed to open file\n";
            return false;;
        }
        return true;
    }
    void printHexPacket(std::vector<uint8_t>& data) {
        std::cout <<"\n";
        for (size_t i = 0; i < data.size(); ++i) {
            printf("%02X ", data[i]);
            if ((i + 1) % 16 == 0) printf("\n");
        }
        printf("\n");
    }
    std::string bytes_to_hex(std::vector<uint8_t>& data) {
        std::ostringstream oss;
        for (auto b : data) {
            oss << std::hex << std::setw(2) << std::setfill('0') << (int)b;
            }
        return oss.str();
    }
}