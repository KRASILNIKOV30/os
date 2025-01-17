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

set(BOOST_ROOT "/usr/include/boost")

find_package(Boost REQUIRED COMPONENTS system)
find_package(wxWidgets REQUIRED COMPONENTS core base)
if(wxWidgets_USE_FILE)
    include(${wxWidgets_USE_FILE})
endif()


add_executable(whiteboard
        whiteboard/main.cpp
        whiteboard/WhiteboardServer.h
        whiteboard/WhiteboardClient.h
)

target_link_libraries(calc PUBLIC osWrappers)
target_link_libraries(whiteboard
        Boost::system
        ${wxWidgets_LIBRARIES}
)

set_target_properties(calc PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(whiteboard PROPERTIES LINKER_LANGUAGE CXX)

