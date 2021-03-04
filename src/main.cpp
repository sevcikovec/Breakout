// PA199_project.cpp : Defines the entry point for the application.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "lodepng.h"
#include "main.h"
#include "renderer/shader.h"

using namespace std;

int main(void)
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
    const char* filename = "E:/Honza/PA199/pa199_project/resources/Lenna.png";
    //const char* filename = "/../../../../resources/Lenna.png";
    std::vector<unsigned char> image;
    unsigned width, height;
    unsigned error = lodepng::decode(image, width, height, filename);
    
    std::cout << error << std::endl;

    const char* vertexShader = "E:/Honza/PA199/pa199_project/resources/shaders/vert.glsl";
    const char* fragmentShader = "E:/Honza/PA199/pa199_project/resources/shaders/frag.glsl";
    Shader testShader(vertexShader, fragmentShader);

   

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // test shader uniform binding
        testShader.Bind();
        testShader.SetUniformFloat4("color", 0.0f, 0.5f, 0.8f, 1);
        
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}