// PA199_project.cpp : Defines the entry point for the application.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "lodepng.h"
#include "Renderer/Shader.h"
#include "Math/Vec3.h"
#include <iostream>

using namespace std;

namespace Engine {

    int Start(void)
    {
        GLFWwindow* window;

        /* Initialize the library */
        if (!glfwInit())
            return -1;


        /* Create a windowed mode window and its OpenGL context */
        window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
        if (!window)
        {
            glfwTerminate();
            return -1;
        }


        /* Make the window's context current */
        glfwMakeContextCurrent(window);


        if (GLEW_OK != glewInit()) {
            std::cout << "Error while calling glewInit() " << std::endl;
        }


        // load image, jsut for testing purposes
        const char* filename = "..\\..\\..\\..\\Engine\\resources\\Lenna.png";
        std::vector<unsigned char> image;
        unsigned width, height;
        unsigned error = lodepng::decode(image, width, height, filename);

        std::cout << error << std::endl;

        const char* vertexShader = "..\\..\\..\\..\\Engine\\resources\\shaders\\vert.glsl";
        const char* fragmentShader = "..\\..\\..\\..\\Engine\\resources\\shaders\\frag.glsl";
        Shader testShader(vertexShader, fragmentShader);

        Vec3 color(1, 0, 0);
        double startTime = glfwGetTime();
        double currentTime = startTime;

        double frameTime = 0;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            frameTime = glfwGetTime() - currentTime;
            currentTime = glfwGetTime();

            // animate color
            color.x = sin(currentTime) / 2.f + .5f;
            color.y = cos(currentTime) / 2.f + .5f;

            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            // test shader uniform binding
            testShader.Bind();
            testShader.SetUniformFloat4("color", color.x, color.y, color.z, 1);

            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

        glfwTerminate();
        return 0;
    }
}