cmake_minimum_required(VERSION 3.13)
project(acaproject)

set(CMAKE_CXX_STANDARD 14)

include(CMakeLists.local.txt OPTIONAL)

find_package(OpenCV REQUIRED)

find_package(OpenMP REQUIRED)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

add_executable(acaproject main.cpp edgedetector/common.h edgedetector/omp/omp_sobel.cpp edgedetector/operators/sobel.cpp edgedetector/edge_detector.h edgedetector/operators/canny.cpp)
target_link_libraries(acaproject ${OpenCV_LIBS})