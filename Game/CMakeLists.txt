project(Game)

file(GLOB_RECURSE SOURCES 
    *.h *.cpp
     )

add_executable(Game ${SOURCES})

file(COPY "resources" DESTINATION "assets")

# link libraries
target_link_libraries(Game Engine)