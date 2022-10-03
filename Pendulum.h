#pragma once
#define _USE_MATH_DEFINES

#include <math.h>
#include <vector>
#include <iostream>
#include <glad/glad.h>

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

struct Beam
{
    Beam(float p_mass = 1.0f, float p_l = 1.0f, float p_theta = M_PI / 2, float p_omega = 0.0f) :
        x(0.0f),
        y(0.0f),
        z(0.0f),
        mass(p_mass),
        l(p_l),
        theta(p_theta),
        omega(p_omega)
    {
        x = l * sin(theta) / 2;
        y = -l * cos(theta) / 2;
    };

    float x;
    float y;
    float z;

    float mass;
    float l;
    float theta;
    float omega;
};

class Pendulum
{
public:
    Pendulum(float* mass_beams, float* l_beams, float* theta_beams, float* omega_beams, unsigned int num_beams = 2);
    ~Pendulum();

    void calculatePhysicalModel(float step);

    unsigned int countBeams() const { return beams.size(); }

    GLfloat* getDrawVertices() const { return vertices; }
    GLuint countDrawVertices() const { return countBeams() * 4 * 3; }
    void calculateDrawVertices();

    GLuint* getEdgeIndices() const { return edge_indices; }
    GLuint countEdgeIndices() const { return countBeams() * 4; }

    GLuint* getSurfaceIndices() const { return surface_indices; }
    GLsizeiptr countSurfaceIndices() const { return countBeams() * 2 * 3; }

    void createBuffers();
    void deleteBuffers();

    void drawEdges();
    void drawSurface();

protected:

    std::vector<Beam> beams;

    GLfloat* vertices = nullptr;
    GLuint* edge_indices = nullptr;
    GLuint* surface_indices = nullptr;

    VAO vao;
    VBO vbo;
    EBO edge_ebo;
    EBO surface_ebo;
};

//void calculateRungeKutta();
