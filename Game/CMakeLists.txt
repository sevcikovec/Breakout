project(Game)


add_executable(Game 
"Main.cpp" 
"Breakout.cpp"
"Breakout.h" 
"BreakoutScene.cpp"
"BreakoutScene.h"

"Components/PlayerComponent.h"
"Systems/PlayerMovementSystem.h"

 "Scripts/TestScript.cpp")


# link libraries
target_link_libraries(Game Engine)