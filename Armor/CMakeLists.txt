cmake_minimum_required(VERSION 3.5)
project(Armor)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
find_package(OpenCV REQUIRED)

add_executable(Armor ${SOURCE_FILES})
target_link_libraries(Armor ${OpenCV_LIBS})
