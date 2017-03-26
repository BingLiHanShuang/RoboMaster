cmake_minimum_required(VERSION 3.5)
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")
project(stm32)



set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.c protocol.c protocol.h protocol.pb-c.c protocol.pb-c.h protobuf-c.h protobuf-c.c DataType.h protocol_config.h)
add_executable(stm32 ${SOURCE_FILES})
