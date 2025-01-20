cmake_minimum_required(VERSION 3.26)
project(os)

set(CMAKE_CXX_STANDARD 20)

enable_testing()

# add_subdirectory(lab1)
# add_subdirectory(lab2)
# add_subdirectory(lab3)
# add_subdirectory(lab4)
add_subdirectory(lab5)
add_subdirectory(lab6)
add_subdirectory(lab7)
add_subdirectory(lab8)
add_subdirectory(lab9)
add_subdirectory(lab10-crossplatform)
add_subdirectory(exam)
add_subdirectory(nfa_to_dfa)

add_library(catch2 INTERFACE)
add_library(timer INTERFACE)
add_library(coutBuffer INTERFACE)
add_library(osWrappers INTERFACE
        lab9/calculator/main.cpp)

target_include_directories(
        catch2 INTERFACE lib/catch2/
        timer INTERFACE lib/timer/
        coutBuffer INTERFACE lib/coutBuffer/
        osWrappers INTERFACE lib/osWrappers/
)


