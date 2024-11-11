set(CMAKE_CXX_STANDARD 20)

add_executable(sys-info-win
        sysInfoWin/SysInfoWin.hpp
        sysInfoWin/SysInfoWin.cpp
)

add_executable(sys-info-linux
        sysInfoLinux/SysInfoLinux.hpp
        sysInfoLinux/SysInfoLinux.cpp
)

add_executable(sys-info
        sysInfo/main.cpp
        sysInfo/SysInfo.h
        sysInfo/SysInfo.cpp
)
