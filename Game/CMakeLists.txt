project(Game)


add_executable(Game 
"Main.cpp" 
"Breakout.cpp"
"Breakout.h" 
"BreakoutScene.cpp"
"BreakoutScene.h"
 "Scripts/TestScript.cpp")


# link libraries
target_link_libraries(Game Engine)