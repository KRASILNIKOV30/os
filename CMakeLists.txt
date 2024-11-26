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

add_library(catch2 INTERFACE)
add_library(timer INTERFACE)
add_library(coutBuffer INTERFACE)
target_include_directories(catch2 INTERFACE lib/catch2/)
target_include_directories(timer INTERFACE lib/timer/)
target_include_directories(coutBuffer INTERFACE lib/coutBuffer/)
