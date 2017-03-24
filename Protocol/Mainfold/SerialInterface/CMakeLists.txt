cmake_minimum_required(VERSION 2.8)
project(SerialInterface)
find_package (Threads)
set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp UDPServer.cpp UDPServer.h SendQueue.cpp SendQueue.h Serial.h Serial.cpp protocol.cpp protocol.h)
add_executable(C ${SOURCE_FILES})
target_link_libraries (C ${CMAKE_THREAD_LIBS_INIT})
