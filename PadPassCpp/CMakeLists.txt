cmake_minimum_required(VERSION 3.5)
project(PadPassCpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp keras_model.cc keras_model.h)
find_package(OpenCV REQUIRED)

add_executable(PadPassCpp ${SOURCE_FILES})
target_link_libraries(PadPassCpp ${OpenCV_LIBS})
