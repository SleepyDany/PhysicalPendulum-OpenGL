#include "Pendulum.h"

#include "Solver.h"

DoublePendulum::DoublePendulum(float* mass_beams, float* l_beams, float* theta_beams, float* omega_beams)
{
    const int num_beams = 2;

    vertices = new GLfloat[num_beams * 4 * 3] {};
    edge_indices = new GLuint[num_beams * 4] {};
    surface_indices = new GLuint[num_beams * 3 * 2] {};

    beams.reserve(num_beams);

    for (int i = 0; i < num_beams; ++i)
    {
        beams.emplace_back(mass_beams[i], l_beams[i], theta_beams[i], omega_beams[i]);

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

DoublePendulum::~DoublePendulum()
{
    beams.clear();

    delete[] vertices;
    vertices = nullptr;

    delete[] edge_indices;
    edge_indices = nullptr;

    delete[] surface_indices;
    surface_indices = nullptr;
}

void DoublePendulum::updateCoordinates()
{
    for (int i = 0; i < countBeams(); ++i)
    {
        beams[i].x = beams[i].l * sin(beams[i].theta) / 2;
        beams[i].y = -beams[i].l * cos(beams[i].theta) / 2;

        if (i != 0)
        {
            beams[i].x += beams[i - 1].l * sin(beams[i - 1].theta);
            beams[i].y -= beams[i - 1].l * cos(beams[i - 1].theta);
        }
    }
}

void DoublePendulum::calculateDerivates(const float* y_in, float* derivates)
{
    // y_in
    // 0 - theta 1
    // 1 - omega 1
    // 2 - theta 2
    // 3 - omega 2

    float theta1 = y_in[0], theta2 = y_in[2], w1 = y_in[1], w2 = y_in[3];
    const float g = 9.8f;
    
    float m1 = beams[0].mass, l1 = beams[0].l;
    float m2 = beams[1].mass, l2 = beams[1].l;

    const float M = m1 + m2;
    
    const float delta = theta2 - theta1;
    //const float delta = theta1 - theta2;
    float den = M * l1 - m2 * l1 * cos(delta) * cos(delta);
    //float den = l1 * (9 * m2 * pow(cos(delta), 2) - 4 * m1 - 12 * m2);

    derivates[0] = w1;
    derivates[1] = (m1 * l1 * pow(w1, 2) * sin(delta) * cos(delta) +
                    m2 * g * sin(theta2) * cos(delta) + 
                    m2 * l2 * pow(w2, 2) * sin(delta) -
                    M * g * sin(theta1)) / den;
   /* derivates[1] = 3 * (-3 * sin(delta) * cos(delta) * l1 * m2 * pow(w1,2) -
                        -2 * pow(w2,2) * m2 * l2 * sin(delta) -
                        -3 * sin(theta2) * cos(delta) * g * m2 +
                         2 * g * sin(theta1) * m1 +
                         4 * g * sin(theta1) * m2) / den;*/
    derivates[2] = w2;
    den *= l2 / l1;
    derivates[3] = (-m2 * l2 * pow(w2, 2) * sin(delta) * cos(delta) +
                    M * g * sin(theta1) * cos(delta) -
                    M * l1 * pow(w1, 2) * sin(delta) -
                    M * g * sin(theta2)) / den;
    /*derivates[3] = 3 * (-3 * sin(delta) * cos(delta) * l2 * m2 * pow(w2,2) -
                        -2 * sin(delta) * l1 * m1 * pow(w1,2) -
                        -6 * sin(delta) * l1 * m2 * pow(w1,2) +
                         3 * sin(theta1) * cos(delta) * g * m1 +
                         6 * sin(theta1) * cos(delta) * g * m2 - 
                        -2 * sin(theta2) * g * m1 -
                        -6 * sin(theta2) * g * m2) / den;*/
}

//void DoublePendulum::solveODEsRK4(const float* y_in, float* y_out, const unsigned int size, float step)
//{
//    float* derivates = new float[size] {};
//    float* y_temp = new float[size] {};
//    float** k = new float* [4] {};
//
//    for (int i = 0; i < 4; ++i)
//    {
//        k[i] = new float[size] {};
//    }
//
//    // 1
//    calculateDerivates(y_in, derivates);
//    for (int i = 0; i < size; ++i)
//    {
//        k[0][i] = step * derivates[i];
//        y_temp[i] = y_in[i] + k[0][i] / 2;
//    }
//
//    // 2
//    calculateDerivates(y_temp, derivates);
//    for (int i = 0; i < size; ++i)
//    {
//        k[1][i] = step * derivates[i];
//        y_temp[i] = y_in[i] + k[1][i] / 2;
//    }
//
//    // 3
//    calculateDerivates(y_temp, derivates);
//    for (int i = 0; i < size; ++i)
//    {
//        k[2][i] = step * derivates[i];
//        y_temp[i] = y_in[i] + k[2][i];
//    }
//
//    // 4
//    calculateDerivates(y_temp, derivates);
//    for (int i = 0; i < size; ++i)
//    {
//        k[3][i] = step * derivates[i];
//        y_out[i] = y_in[i] + k[0][i] / 6 + k[1][i] / 3 + k[2][i] / 3 + k[3][i] / 6;
//    }
//
//    delete[] derivates;
//    derivates = nullptr;
//
//    delete[] y_temp;
//    y_temp = nullptr;
//
//    for (int i = 0; i < 4; ++i)
//    {
//        delete[] k[i];
//        k[i] = nullptr;
//    }
//    delete[] k;
//    k = nullptr;
//}

void DoublePendulum::calculatePhysicalModel(float step)
{
    if (step <= 0)
    {
        std::cout << "Uncorrect step for calculations." << std::endl;
        return;
    }

    const unsigned int size = 4;
    
    // 0 - theta 1
    // 1 - omega 1
    // 2 - theta 2
    // 3 - omega 2

    float* y_in = new float[size] {};
    float* y_out = new float[size] {};

    for (int i = 0; i < countBeams(); ++i)
    {
        y_in[2 * i] = beams[i].theta;
        y_in[2 * i + 1] = beams[i].omega;
    }

    std::function<void(const float*, float*)> func = std::bind(&DoublePendulum::calculateDerivates, this, std::placeholders::_1, std::placeholders::_2);

    SolverODEs* solver = new SolverODEs();
    solver->setMethod(SolverODEs::RungeKutta4);
    solver->setStep(step);

    solver->SolveRK4(y_in, func, y_out, size);

    //solveODEsRK4(y_in, y_out, size, step);

    for (int i = 0; i < countBeams(); ++i)
    {
        beams[i].theta = y_out[2 * i] - floor(y_out[2 * i] / (2 * M_PI)) * 2 * M_PI; // Round theta in [0; 2*PI]
        beams[i].omega = y_out[2 * i + 1];
    }

    updateCoordinates();

    calculateDrawVertices();
}

void vec_summ(float* result, const float* vec1, const float* vec2, unsigned int size)
{
    if (!vec1 || !vec2 || !result)
        return;

    for (unsigned int i = 0; i < size; ++i)
        result[i] = vec1[i] + vec2[i];
};

void vec_subtract(float* result, const float* vec1, const float* vec2, unsigned int size)
{
    if (!vec1 || !vec2 || !result)
        return;

    for (unsigned int i = 0; i < size; ++i)
        result[i] = vec1[i] - vec2[i];
};

void DoublePendulum::calculateDrawVertices()
{
    float width = 0.02f;

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

        vec_summ(temp_vec, L, W, 2);
        vec_summ(vertices + offset, center, temp_vec, 2);
        offset += 3;

        vec_subtract(temp_vec, L, W, 2);
        vec_summ(vertices + offset, center, temp_vec, 2);
        offset += 3;

        L[0] *= -1; L[1] *= -1;
        vec_subtract(temp_vec, L, W, 2);
        vec_summ(vertices + offset, center, temp_vec, 2);
        offset += 3;

        vec_summ(temp_vec, W, L, 2);
        vec_summ(vertices + offset, center, temp_vec, 2);
        offset += 3;
    }
}

void DoublePendulum::createBuffers()
{
    vao.Bind();

    vbo.uploadBufferData(vertices, countDrawVertices() * sizeof(GLfloat));
    vbo.Bind();

    edge_ebo.uploadBufferData(edge_indices, countEdgeIndices() * sizeof(GLuint)),
    surface_ebo.uploadBufferData(surface_indices, countSurfaceIndices() * sizeof(GLuint));

    vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

    vao.Unbind();
    vbo.Unbind();
    edge_ebo.Unbind();
    surface_ebo.Unbind();
}

void DoublePendulum::deleteBuffers()
{
    vao.Delete();
    vbo.Delete();
    edge_ebo.Delete();
    surface_ebo.Delete();
}

void DoublePendulum::drawEdges()
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

void DoublePendulum::drawSurface()
{
    vao.Bind();
    surface_ebo.Bind();

    glDrawElements(GL_TRIANGLES, countSurfaceIndices(), GL_UNSIGNED_INT, 0);

    surface_ebo.Unbind();
    vao.Unbind();
}

void DoublePendulum::draw(GLuint program_id)
{
    createBuffers();

    GLuint uColorID = glGetUniformLocation(program_id, "uColor");
    glUniform3f(uColorID, 1.0f, 0.0f, 0.0f);
    drawSurface();

    glUniform3f(uColorID, 153.f / 255.f, 0.0f, 0.0f);
    drawEdges();
}
