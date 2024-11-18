set(CMAKE_CXX_STANDARD 20)

add_executable(treadsafe-set
        treadsafeSet/TreadsafeSet.h
        treadsafeSet/main.cpp
        treadsafeSet/ISet.h
        treadsafeSet/Timer.h
)

add_executable(lab5-tests
        threadPool/ThreadPool.h
        bgThreadDispatcher/BgThreadDispatcher.h
        tests/BgThreadDispatcher-tests.cpp
        tests/ThreadPool-tests.cpp
)

add_executable(search-server-tests
        threadPool/ThreadPool.h
        bgThreadDispatcher/BgThreadDispatcher.h
        tests/BgThreadDispatcher-tests.cpp
        tests/ThreadPool-tests.cpp
        searchServer/Path.h
        searchServer/Document.h
        searchServer/InvertedIndex.h
        tests/InversedIndex_tests.cpp
)

set_target_properties(treadsafe-set PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(lab5-tests PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(lab5-tests PUBLIC catch2)
target_link_libraries(search-server-tests PUBLIC catch2)

add_test(NAME lab5-tests COMMAND lab5-tests)
add_test(NAME search-server-tests COMMAND search-server-tests)
