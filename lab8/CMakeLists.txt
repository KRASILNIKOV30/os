add_executable(snowflakes
        snowflakes/main.cpp
        snowflakes/DisplayWrapper.h
        snowflakes/GCWrapper.h
        snowflakes/Timer.h
)

find_package(X11 REQUIRED)

if (X11_FOUND)
    include_directories(${X11_INCLUDE_DIR})
    message(STATUS "X11_INCLUDE_DIR: ${X11_INCLUDE_DIR}")
    message(STATUS "X11_LIBRARIES: ${X11_LIBRARIES}")
else ()
    message(FATAL_ERROR "X11 library not found")
endif ()

target_link_libraries(snowflakes ${X11_LIBRARIES})