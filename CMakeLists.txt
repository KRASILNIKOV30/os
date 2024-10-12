cmake_minimum_required(VERSION 3.26)
project(os)

set(CMAKE_CXX_STANDARD 20)

add_executable(sys-info-win
        lab1/sysInfoWin/SysInfoWin.hpp
        lab1/sysInfoWin/SysInfoWin.cpp
)

add_executable(sys-info-linux
        lab1/sysInfoLinux/SysInfoLinux.hpp
        lab1/sysInfoLinux/SysInfoLinux.cpp
)

add_executable(sys-info
        lab1/sysInfo/main.cpp
        lab1/sysInfo/SysInfo.h
        lab1/sysInfo/SysInfo.cpp
)

add_executable(flip-case
        lab2/flipCase/flip-case.cpp
)
add_executable(list-processes-linux lab2/listProcesses/list-processes-linux.cpp)

add_executable(list-processes-win lab2/listProcesses/list-processes-win.cpp)

add_executable(zombie lab2/zombie/zombie.cpp)

add_executable(signal-handler lab3/signal-handler.cpp)
add_executable(kill-child lab3/kill-child.cpp)
add_executable(pipes lab3/pipes.cpp)
