cmake_minimum_required(VERSION 2.8)
project(UltraSonic)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -std=c99")
set(SOURCE_FILES main.cpp DJI_guidance.h DJI_utility.cpp DJI_utility.h protocol.c protocol.pb-c.h protocol.c protocol.pb-c.c protocol.h protobuf-c.h protobuf-c.c protocol.cpp UDPClient.h UDPClient.cpp)

add_executable(UltraSonic ${SOURCE_FILES})
target_link_libraries(UltraSonic -lDJI_guidance -lusb-1.0 -lpthread)