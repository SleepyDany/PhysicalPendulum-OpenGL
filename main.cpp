#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main()
{
    // Initialize GLFW
    glfwInit();

    // What version of OpenGL GLFW should use (OpenGL 3.3)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // CORE profile (only have the modern functions)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1024, 720, "Physical Pendulum", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return -1;
    }

    // Introduce the window into the current context
    glfwMakeContextCurrent(window);
    
    // Load GLAD so it configures OpenGL
    gladLoadGL();

    // Specify the viewport of OpenGL in the window (from x,y to x1,y1)
    glViewport(0, 0, 1024, 720);

    // Specify color of the background, clean back buffer and assign new color to it
    glClearColor(0.08f, 0.13f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Swap back buffer with front buffer
    glfwSwapBuffers(window);

    while (!glfwWindowShouldClose(window))
    {
        // All GLFW Events
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    return 0;
}