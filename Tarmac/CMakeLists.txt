cmake_minimum_required(VERSION 3.5)
project(Tarmac)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
find_package(OpenCV REQUIRED)

set(SOURCE_FILES main.cpp protobuf-c.c protocol.cpp protocol.h protocol.pb-c.c protobuf-c.h protocol.pb-c.h protocol.c UDPClient.cpp UDPClient.h detection.cpp Structure.cpp Structure.h)
add_executable(Tarmac ${SOURCE_FILES})
target_link_libraries(Tarmac ${OpenCV_LIBS})
