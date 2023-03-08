cmake_minimum_required(VERSION 3.24)
project(TP3)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(TP3
        src/color.h
        src/image_ppm.h
        src/ImageAlgorithms.h
        src/main.cpp
        src/Writer.h)
