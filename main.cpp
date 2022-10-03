#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ShaderClass.h"
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"

#include "Pendulum.h"

int main()
{
    // Initialize GLFW
    glfwInit();

    // What version of OpenGL GLFW should use (OpenGL 3.3)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // CORE profile (only have the modern functions)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(720, 720, "Physical Pendulum", NULL, NULL);
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
    glViewport(0, 0, 720, 720);

    // Creates shader object using shaders default.vert and default.frag
    Shader shader_program("default.vert", "default.frag");

    Pendulum pendulum(new float[2] {1, 1}, new float[2] {0.4f, 0.4f}, new float[2] {M_PI / 2, M_PI / 2}, new float[2] {0.0f, 0.0f});
    pendulum.calculateDrawVertices();
    pendulum.createBuffers();

    GLuint uColorID = glGetUniformLocation(shader_program.ID, "uColor");

    // Tell OpenGL which Shader Program we want to use
    shader_program.Activate();

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUniform3f(uColorID, 1.0f, 0.0f, 0.0f);
        pendulum.drawSurface();

        glUniform3f(uColorID, 153.f / 255.f, 0.0f, 0.0f);
        pendulum.drawEdges();

         glfwSwapBuffers(window);

        // All GLFW Events
        glfwPollEvents();
    }

    pendulum.deleteBuffers();

    shader_program.Delete();

    // Delete window
    glfwDestroyWindow(window);

    // Terminate GLFW
    glfwTerminate();

    return 0;
}