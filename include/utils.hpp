#ifndef UTILS
#define UTILS

#include <fstream>

namespace Utils{
    bool checkIfFile(std::ifstream& file);
    void printHexPacket(std::vector<uint8_t>& data);
    std::string bytes_to_hex(std::vector<uint8_t>& data);
}
#endif
