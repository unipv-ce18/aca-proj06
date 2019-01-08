cmake_minimum_required(VERSION 3.13)
project(acaproject)

set(CMAKE_CXX_STANDARD 14)

include(CMakeLists.local.txt OPTIONAL)

find_package(OpenCV REQUIRED)

add_executable(acaproject main.cpp sobel.h common.h)
target_link_libraries(acaproject ${OpenCV_LIBS})