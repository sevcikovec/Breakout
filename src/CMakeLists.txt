cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (PA199_project 
"main.cpp" "main.h" 
"Renderer/Shader.h" "Renderer/Shader.cpp"
"Math/Vec3.h" "Math/Vec3.cpp")


# gind OPENGL
find_package(OpenGL REQUIRED)





# add glew as static lib
add_library(glew32s STATIC IMPORTED)
set_target_properties(glew32s PROPERTIES
  IMPORTED_LOCATION  "${CMAKE_SOURCE_DIR}/Dependencies/Glew/lib/Release/x64/glew32s.lib"
  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/Dependencies/Glew/include/"
)
add_compile_definitions(GLEW_STATIC)

# link libraries
target_link_libraries(PA199_project glfw)
target_link_libraries(PA199_project OpenGL::GL)
target_link_libraries(PA199_project lodepng)
target_link_libraries(PA199_project glew32s)


target_include_directories(PA199_project PUBLIC "../Dependencies/lodepng/lodepng/include/")