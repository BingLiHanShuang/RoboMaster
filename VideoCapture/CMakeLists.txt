cmake_minimum_required(VERSION 2.8)
project(VideoCapture)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES avilib.c avilib.h uvcvideo.h v4l2uvc.c v4l2uvc.h main.cpp UDPClient.cpp UDPServer.cpp protobuf-c.c protocol.pb-c.c ReceiveHandler.cpp ReceiveHandler.h protocol.pb-c.h VideoRecorder.cpp VideoRecorder.h DataType.h protocol.c protocol.h protocol.cpp)
find_package(OpenCV REQUIRED)

add_executable(VideoCapture ${SOURCE_FILES})
target_link_libraries(VideoCapture ${OpenCV_LIBS})