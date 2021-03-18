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
       


        /*

        Vec3 color(1, 0, 0);
        double startTime = glfwGetTime();
        double currentTime = startTime;

        double frameTime = 0;
        while (!glfwWindowShouldClose(window))
        {
            frameTime = glfwGetTime() - currentTime;
            currentTime = glfwGetTime();

            // animate color
            color.x = sin(currentTime) / 2.f + .5f;
            color.y = cos(currentTime) / 2.f + .5f;


            glClear(GL_COLOR_BUFFER_BIT);

            // test shader uniform binding
            testShader.Bind();
            testShader.SetUniformFloat4("color", color.x, color.y, color.z, 1);

            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

            // Swap front and back buffers
            glfwSwapBuffers(window);

            // Poll for and process events 
            glfwPollEvents();
        }

        glfwTerminate();
        return 0;
        */
   
}