project(Game)

file(GLOB_RECURSE SOURCES 
    *.h *.cpp
     )

add_executable(Game ${SOURCES})


# link libraries
target_link_libraries(Game Engine)