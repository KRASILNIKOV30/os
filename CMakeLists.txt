cmake_minimum_required(VERSION 3.26)
project(os)

set(CMAKE_CXX_STANDARD 20)

add_executable(sys-info-win
        lab1/SysInfoWin/SysInfoWin.hpp
        lab1/SysInfoWin/SysInfoWin.cpp
)

add_executable(sys-info-linux
        lab1/SysInfoLinux/SysInfoLinux.hpp
        lab1/SysInfoLinux/SysInfoLinux.cpp
)

add_executable(sys-info
        lab1/SysInfo/main.cpp
        lab1/SysInfo/SysInfo.h
        lab1/SysInfo/SysInfo.cpp
)
