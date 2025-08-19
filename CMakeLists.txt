cmake_minimum_required(VERSION 3.10)
project(traceroute VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
add_executable(traceroute src/main.cpp src/packet_crafting.cpp src/transmission_socket.cpp src/probe.cpp src/utils.cpp)

