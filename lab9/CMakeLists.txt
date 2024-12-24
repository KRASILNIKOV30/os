set(CMAKE_CXX_STANDARD 20)

add_executable(calc
        calculator/Acceptor.h
        ../lib/osWrappers/FileDesc.h
        calculator/Server.h
        calculator/Client.h
        calculator/constants.h
        calculator/main.cpp
        calculator/Connection.h
)

target_link_libraries(calc PUBLIC osWrappers)

set_target_properties(calc PROPERTIES LINKER_LANGUAGE CXX)