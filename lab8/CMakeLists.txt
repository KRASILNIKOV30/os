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

find_package(wxWidgets REQUIRED COMPONENTS core base)
if(wxWidgets_USE_FILE)
    include(${wxWidgets_USE_FILE})
endif()

add_executable(todo-list
        todoList/Model.h
        todoList/SignallingValue.h
        todoList/ViewModel.h
        todoList/View.h
        todoList/main.cpp
        todoList/
)

target_link_libraries(todo-list
        ${wxWidgets_LIBRARIES}
)

set_target_properties(todo-list PROPERTIES LINKER_LANGUAGE CXX)

