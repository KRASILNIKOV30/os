set(CMAKE_CXX_STANDARD 20)

add_executable(flip-case
        flipCase/flip-case.cpp
)
add_executable(list-processes-linux listProcesses/list-processes-linux.cpp)

add_executable(list-processes-win listProcesses/list-processes-win.cpp)

add_executable(zombie zombie/zombie.cpp)