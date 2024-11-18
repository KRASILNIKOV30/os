set(CMAKE_CXX_STANDARD 20)

add_executable(lab6-tests
        memoryManager/MemoryManager.h
        tests/MemoryManager_tests.cpp
        tests/ThreadPool.h
)

add_executable(histogram
        histogram/Histogram.cpp
        histogram/FileDesc.h
        histogram/ThreadsafeVector.h
)



target_link_libraries(lab6-tests PUBLIC catch2)
target_link_libraries(histogram PUBLIC timer)
target_link_libraries(search-server-tests PUBLIC catch2)
add_test(NAME lab6-tests COMMAND lab6-tests)
add_test(NAME search-server-tests COMMAND search-server-tests)
