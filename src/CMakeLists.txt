# CMakeList.txt : CMake project for PA199_project, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (PA199_project "main.cpp" "main.h")

# TODO: Add tests and install targets if needed.
find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)


target_link_libraries(PA199_project glfw)
target_link_libraries(PA199_project OpenGL::GL)

