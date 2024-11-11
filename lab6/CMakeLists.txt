set(CMAKE_CXX_STANDARD 20)

add_executable(lab6-tests
        memoryManager/MemoryManager.h
        tests/MemoryManager_tests.cpp
)

target_link_libraries(lab6-tests PUBLIC catch2)
add_test(NAME lab6-tests COMMAND lab6-tests)