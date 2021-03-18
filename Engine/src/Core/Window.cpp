#include "Window.h"
#include <cassert>

namespace Engine {
	Window::Window(const std::string& name)
	{

        /* Initialize the library */
        if (!glfwInit())
            assert(false);


        /* Create a windowed mode window and its OpenGL context */
        window = glfwCreateWindow(640, 480, name.c_str(), NULL, NULL);
        if (!window)
        {
            glfwTerminate();
            assert(false);
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(window);
	}

    void Window::OnUpdate()
    {
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
	GLFWwindow* Window::GetGLFW()
	{
		return window;
	}
}