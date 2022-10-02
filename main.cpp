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
    {   // Coords                                      // Colors
        -0.5f, -0.5f * float(sqrt(3)) / 3,     0.0f,   0.8f, 0.3f,  0.02f,
         0.5f, -0.5f * float(sqrt(3)) / 3,     0.0f,   0.8f, 0.3f,  0.02f,
         0.0f,  0.5f * float(sqrt(3)) * 2 / 3, 0.0f,   1.0f, 0.6f,  0.32f,
        -0.25f, 0.5f * float(sqrt(3)) / 6,     0.0f,   0.9f, 0.45f, 0.17f,
         0.25f, 0.5f * float(sqrt(3)) / 6,     0.0f,   0.9f, 0.45f, 0.17f,
         0.0f, -0.5f * float(sqrt(3)) / 3,     0.0f,   0.8f, 0.3f,  0.02f
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

    // Links VBO attributes (coords/colors) to VAO
    vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
    vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    // Unbind all to prevent accidentally modifying
    vao.Unbind();
    vbo.Unbind();
    ebo.Unbind();

    // Get ID of uniform called "scale"
    GLuint scaleID = glGetUniformLocation(shader_program.ID, "scale");

    // Tell OpenGL which Shader Program we want to use
    shader_program.Activate();
    // Assign a value to uniform "scale": MUST be done after Activate of shader program
    glUniform1f(scaleID, 0.5f);

    // Bind Vertex Array Object
    vao.Bind();

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.99f, 0.99f, 0.99f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

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

    shader_program.Delete();

    // Delete window
    glfwDestroyWindow(window);

    // Terminate GLFW
    glfwTerminate();

    return 0;
}