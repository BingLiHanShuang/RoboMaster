cmake_minimum_required(VERSION 3.5)
project(stm32)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp protocol.c protocol.h protocol.pb-c.c protocol.pb-c.h protobuf-c.h protobuf-c.c)
add_executable(stm32 ${SOURCE_FILES})
