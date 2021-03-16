cmake_minimum_required (VERSION 3.8)

project (Engine)

# Include sub-projects.
add_subdirectory ("Dependencies/lodepng")
add_subdirectory ("Dependencies/glfw")


# add sources
set (SOURCES 
    "src/Math/Mat3.cpp"
    "src/Math/Vec3.cpp"
    "src/Math/Mat4.cpp"
    "src/Math/Vec4.cpp"

    "src/Renderer/Shader.cpp"
    "src/Renderer/Renderer.cpp"
    "src/Renderer/VertexBuffer.cpp"
    "src/Renderer/VertexArray.cpp"

    "src/Core/Application.cpp"
    "src/Core/Window.cpp"

     "src/Scene/Scene.cpp"
     "src/Scene/Systems.cpp"
     "src/Scene/Entity.cpp"
     )

set (API 
    "src/Utils.h"

    "src/Math/Mat3.h"
    "src/Math/Vec3.h"
    "src/Math/Vec4.h"
    "src/Math/Mat4.h"
    "src/Math/Quaternion.h"

    "src/Renderer/Shader.h"
    "src/Renderer/Renderer.h"
    "src/Renderer/VertexBuffer.h"
    "src/Renderer/VertexArray.h"

    "src/Core/Application.h"
    "src/Core/Window.h"

     "src/Scene/Scene.h"
     "src/Scene/Entity.h"
     "src/Scene/Components.h"
     "src/Scene/Systems.h"
     "src/Scene/ECS.h"
     "src/Scene/ScriptableEntity.h"
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
