cmake_minimum_required(VERSION 3.26)
project(os)

set(CMAKE_CXX_STANDARD 20)

add_executable(sys-info-win
        lab1/sys-info-win.cpp
)

add_executable(sys-info-linux
        lab1/sys-info-linux.cpp
)
