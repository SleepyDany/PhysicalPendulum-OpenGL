#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ShaderClass.h"
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"

int main()
{
    // Initialize GLFW
    glfwInit();

    // What version of OpenGL GLFW should use (OpenGL 3.3)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // CORE profile (only have the modern functions)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLfloat vertices[] =
    {
        -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
         0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
         0.0f,  0.5f * float(sqrt(3)) * 2 / 3, 0.0f,
        -0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,
         0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,
         0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f
    };

    GLuint indices[] =
    {
        0, 3, 5,
        3, 2, 4,
        5, 4, 1
    };

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

    // Creates shader object using shaders default.vert and default.frag
    Shader shader_program("default.vert", "default.frag");

    // Generates Vertex Array Object and binds it
    VAO vao;
    vao.Bind();

    // Generates Vertex Buffer Object and links it to vertices
    VBO vbo(vertices, sizeof(vertices));
    // Generates Element Buffer Object and links it to indices
    EBO ebo(indices, sizeof(indices));

    // Links VBO to VAO
    vao.LinkVBO(vbo, 0);
    // Unbind all to prevent accidentally modifying
    vao.Unbind();
    vbo.Unbind();
    ebo.Unbind();

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.99f, 0.99f, 0.99f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Tell OpenGL which Shader Program we want to use
        shader_program.Activate();

        // Bind Vertex Array Object
        vao.Bind();

        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);

        // All GLFW Events
        glfwPollEvents();
    }

    // Delete all created objects
    vao.Delete();
    vbo.Delete();
    ebo.Delete();

    // Delete window
    glfwDestroyWindow(window);

    // Terminate GLFW
    glfwTerminate();

    return 0;
}