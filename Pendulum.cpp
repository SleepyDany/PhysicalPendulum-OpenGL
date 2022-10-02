#include "Pendulum.h"
#include <glad/glad.h>

Pendulum::Pendulum(float* mass_beams, float* l_beams, float* theta_beams, unsigned int num_beams) :
    vbo(vertices, countDrawVertices() * sizeof(GLfloat)),
    edge_ebo(edge_indices, countEdgeIndices() * sizeof(GLuint)),
    surface_ebo(surface_indices, countSurfaceIndices() * sizeof(GLuint))
{
    vertices = new GLfloat[num_beams * 4 * 3] {};
    edge_indices = new GLuint[num_beams * 4] {};
    surface_indices = new GLuint[num_beams * 3 * 2] {};

    beams.reserve(num_beams);

    for (int i = 0; i < num_beams; ++i)
    {
        beams.emplace_back(mass_beams[i], l_beams[i], theta_beams[i]);

        if (i != 0)
        {
            beams[i].x += beams[i - 1].l * sin(beams[i - 1].theta);
            beams[i].y -= beams[i - 1].l * cos(beams[i - 1].theta);
        }

        edge_indices[4 * i] = 4 * i;
        edge_indices[4 * i + 1] = 4 * i + 1;
        edge_indices[4 * i + 2] = 4 * i + 2;
        edge_indices[4 * i + 3] = 4 * i + 3;

        surface_indices[6 * i] = 4 * i;
        surface_indices[6 * i + 1] = 4 * i + 1;
        surface_indices[6 * i + 2] = 4 * i + 2;

        surface_indices[6 * i + 3] = 4 * i;
        surface_indices[6 * i + 4] = 4 * i + 2;
        surface_indices[6 * i + 5] = 4 * i + 3;
    }
}

Pendulum::~Pendulum()
{
    beams.clear();

    delete[] vertices;
    vertices = nullptr;

    delete[] edge_indices;
    edge_indices = nullptr;

    delete[] surface_indices;
    surface_indices = nullptr;
}

void Pendulum::calculatePhysicalModel(float step)
{

}

void Pendulum::calculateDrawVertices()
{
    float width = 0.02f;

    auto vec_summ = [](float* result, float* vec1, float* vec2)
    { 
        result[0] = vec1[0] + vec2[0];
        result[1] = vec1[1] + vec2[1];
    };

    auto vec_subtract = [](float* result, float* vec1, float* vec2)
    {
        result[0] = vec1[0] - vec2[0];
        result[1] = vec1[1] - vec2[1];
    };

    float L[2] = {};
    float W[2] = {};

    float center[2] = {};

    int offset = 0;
    for (auto& beam : beams)
    {
        center[0] = beam.x;
        center[1] = beam.y;

        L[0] = -beam.l * sin(beam.theta) / 2;
        L[1] = beam.l * cos(beam.theta) / 2;

        W[0] = width * cos(beam.theta) / 2;
        W[1] = width * sin(beam.theta) / 2;

        float temp_vec[2]{};

        vec_summ(temp_vec, L, W);
        vec_summ(vertices + offset, center, temp_vec);
        offset += 3;

        vec_subtract(temp_vec, L, W);
        vec_summ(vertices + offset, center, temp_vec);
        offset += 3;

        L[0] *= -1; L[1] *= -1;
        vec_subtract(temp_vec, L, W);
        vec_summ(vertices + offset, center, temp_vec);
        offset += 3;

        vec_summ(temp_vec, W, L);
        vec_summ(vertices + offset, center, temp_vec);
        offset += 3;
    }
}

void Pendulum::createBuffers()
{
    vao.Bind();

    vbo = VBO(vertices, countDrawVertices() * sizeof(GLfloat));
    vbo.Bind();

    edge_ebo = EBO(edge_indices, countEdgeIndices() * sizeof(GLuint)),
    surface_ebo = EBO(surface_indices, countSurfaceIndices() * sizeof(GLuint));

    vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

    vao.Unbind();
    vbo.Unbind();
    edge_ebo.Unbind();
    surface_ebo.Unbind();
}

void Pendulum::deleteBuffers()
{
    vao.Delete();
    vbo.Delete();
    edge_ebo.Delete();
    surface_ebo.Delete();
}

void Pendulum::drawEdges()
{
    vao.Bind();
    edge_ebo.Bind();

    for (int i = 0; i < 2; ++i)
    {
        glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, (void*)(4 * i * sizeof(GLuint)));
    }

    edge_ebo.Unbind();
    vao.Unbind();
}

void Pendulum::drawSurface()
{
    vao.Bind();
    surface_ebo.Bind();

    glDrawElements(GL_TRIANGLES, countSurfaceIndices(), GL_UNSIGNED_INT, 0);

    surface_ebo.Unbind();
    vao.Unbind();
}
