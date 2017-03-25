cmake_minimum_required(VERSION 3.5)
project(VideoCapture)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp UDPClient.cpp UDPServer.cpp protobuf-c.c protocol.pb-c.c ReceiveHandler.cpp ReceiveHandler.h protocol.pb-c.h VideoRecord.cpp VideoRecord.h)
find_package(OpenCV REQUIRED)

add_executable(VideoCapture ${SOURCE_FILES})
target_link_libraries(VideoCapture ${OpenCV_LIBS})