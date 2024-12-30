set(STUDENTREPO_SOURCES
    ./include/studentrepo.h
    ./src/studentrepo.cpp
    ./src/StudentRepo.h
    ./src/StudentRepo.cpp
)

add_library(studentrepo SHARED ${STUDENTREPO_SOURCES})

set_target_properties(studentrepo
    PROPERTIES
        C_VISIBILITY_PRESET hidden
        CXX_VISIBILITY_PRESET hidden
)

target_compile_definitions(studentrepo
    PRIVATE
        STUDENTREPO_LIBRARY_BUILD
)

target_include_directories(studentrepo
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)