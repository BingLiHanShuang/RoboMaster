cmake_minimum_required(VERSION 2.8)
project(PadPassCpp)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -O3")

set(SOURCE_FILES main.cpp keras_model.cc keras_model.h protobuf-c.c protocol.pb-c.h protocol.c protocol.cpp protocol.h protocol.pb-c.c protocol.pb-c.h UDPClient.cpp UDPClient.h
         Structure.cpp Structure.h)
find_package(OpenCV REQUIRED)

add_executable(PadPassCpp ${SOURCE_FILES})
target_link_libraries(PadPassCpp ${OpenCV_LIBS})
