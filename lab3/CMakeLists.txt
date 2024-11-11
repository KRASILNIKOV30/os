set(CMAKE_CXX_STANDARD 20)

add_executable(signal-handler signal-handler.cpp)
add_executable(kill-child kill-child.cpp)

add_executable(pipes pipes/pipes.cpp
        pipes/pipes.cpp
        pipes/FileDesc.h
        pipes/Request.h
        pipes/Child.h
        pipes/ProcessManager.h
        pipes/Fetcher.h
)

add_executable(pipes_tests
        pipes/pipes_tests.cpp
        pipes/FileDesc.h
        pipes/Request.h
        pipes/Child.h
        pipes/ProcessManager.h
        pipes/Fetcher.h
)

target_link_libraries(pipes_tests PUBLIC catch2)