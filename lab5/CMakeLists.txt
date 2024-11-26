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
        searchServer/SearchServer.h
        tests/SearchServer_tests.cpp
        searchServer/StrToLower.h
        searchServer/Indexer.h
)

add_executable(search-server
        threadPool/ThreadPool.h
        searchServer/Path.h
        searchServer/Document.h
        searchServer/InvertedIndex.h
        searchServer/SearchServer.h
        searchServer/main.cpp
        searchServer/Menu.h
        searchServer/StrToLower.h
        searchServer/Indexer.h
)

set_target_properties(treadsafe-set PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(lab5-tests PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(search-server PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(lab5-tests PUBLIC catch2)
target_link_libraries(search-server-tests PUBLIC catch2)
target_link_libraries(search-server-tests PUBLIC coutBuffer)

target_link_libraries(search-server PUBLIC timer)

add_test(NAME lab5-tests COMMAND lab5-tests)
add_test(NAME search-server-tests COMMAND search-server-tests)
