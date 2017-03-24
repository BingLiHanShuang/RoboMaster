cmake_minimum_required(VERSION 2.8)
project(SerialInterface)
find_package (Threads)
set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp UDPServer.cpp UDPServer.h SerialSendQueue.cpp SerialSendQueue.h Serial.h Serial.cpp protocol.cpp protocol.h protocol.pb-c.c protocol.h protocol.pb-c.h protobuf-c.c protobuf-c.h UDPClient.cpp UDPClient.h SerialReceive.cpp SerialReceive.h)
add_executable(SerialInterface ${SOURCE_FILES})
target_link_libraries (SerialInterface ${CMAKE_THREAD_LIBS_INIT})
