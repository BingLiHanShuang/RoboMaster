cmake_minimum_required(VERSION 3.5)
project(QRCode)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp protocol.pb.cc protocol.cpp protocol.h protocol.pb.h Serial.cpp Serial.h)
find_package(OpenCV REQUIRED)
find_package(ZBar)
find_package(Protobuf REQUIRED)

include(FindProtobuf)
include_directories(${PROTOBUF_INCLUDE_DIR})
add_executable(QRCode ${SOURCE_FILES})
target_link_libraries(QRCode ${OpenCV_LIBS} ${ZBAR_LIBRARIES}zbar ${PROTOBUF_LIBRARY})
