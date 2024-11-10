set(CMAKE_CXX_STANDARD 20)

add_executable(treadsafe-set
        treadsafeSet/TreadsafeSet.h
        treadsafeSet/main.cpp
        treadsafeSet/ISet.h
        treadsafeSet/Timer.h
)
set_target_properties(treadsafe-set PROPERTIES LINKER_LANGUAGE CXX)