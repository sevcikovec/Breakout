#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "lodepng.h"
#include "../Renderer/Shader.h"
#include "../Math/Vec3.h"
#include <iostream>

#include "Application.h";

using namespace std;

namespace Engine {

    Application::Application(const std::string& name) : window(name)
    {
        this->name = name;

        if (GLEW_OK != glewInit()) {
            std::cout << "Error while calling glewInit() " << std::endl;
        }

        Renderer::Init();
    }

    void Application::Run()
    {
        isRunning = true;

        float lastFrameTime = glfwGetTime();
        while (isRunning) {
            float time = glfwGetTime();
            float frameTime = time - lastFrameTime;
            lastFrameTime = time;

            for(auto scene : scenes) {
                scene->OnUpdate(frameTime);
            }

            window.OnUpdate();
        }
    }


    void Application::AddScene(Scene* scene) {
        scenes.push_back(scene);
    }
        
   
}