cmake_minimum_required (VERSION 3.8)

project (Engine)

# Include sub-projects.
add_subdirectory ("Dependencies/lodepng")
add_subdirectory ("Dependencies/glfw")


# add sources
file(GLOB_RECURSE SOURCES 
    *.cpp
     )

file(GLOB_RECURSE API 
    *.h
     )


add_library(Engine STATIC ${SOURCES} ${API})

set_target_properties(Engine PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/src"
)


# find OPENGL
find_package(OpenGL REQUIRED)


# add glew as static lib
add_library(glew32s STATIC IMPORTED)
set_target_properties(glew32s PROPERTIES
  IMPORTED_LOCATION  "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/Glew/lib/Release/x64/glew32s.lib"
  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/Glew/include/"
)
add_compile_definitions(GLEW_STATIC)

# link libraries
target_link_libraries(Engine glfw)
target_link_libraries(Engine OpenGL::GL)
target_link_libraries(Engine lodepng)
target_link_libraries(Engine glew32s)


target_include_directories(Engine PUBLIC "Dependencies/lodepng/lodepng/include/" ${API})
