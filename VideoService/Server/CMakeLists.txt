cmake_minimum_required(VERSION 3.6)
project(VideoServer)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Structure.cpp Structure.h)
add_executable(VideoServer ${SOURCE_FILES})
target_link_libraries(VideoServer -lv4l2 -lpthread)
