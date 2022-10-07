#include "Solver.h"

SolverODEs::SolverODEs() : method_id(Undefined), step(0.005f)
{
}

SolverODEs::~SolverODEs()
{
}

void SolverODEs::SolveRK4(const float* y_in, std::function<void(const float*, float*)>& func, float* y_out, const unsigned int size)
{
    float* derivates = new float[size] {};
    float* y_temp = new float[size] {};
    float** k = new float*[4] {};

    for (int i = 0; i < 4; ++i)
    {
        k[i] = new float[size] {};
    }

    // 1
    func(y_in, derivates);
    for (int i = 0; i < size; ++i)
    {
        k[0][i] = step * derivates[i];
        y_temp[i] = y_in[i] + k[0][i] / 2;
    }

    // 2
    func(y_temp, derivates);
    for (int i = 0; i < size; ++i)
    {
        k[1][i] = step * derivates[i];
        y_temp[i] = y_in[i] + k[1][i] / 2;
    }

    // 3
    func(y_temp, derivates);
    for (int i = 0; i < size; ++i)
    {
        k[2][i] = step * derivates[i];
        y_temp[i] = y_in[i] + k[2][i];
    }

    // 4
    func(y_temp, derivates);
    for (int i = 0; i < size; ++i)
    {
        k[3][i] = step * derivates[i];
        y_out[i] = y_in[i] + k[0][i] / 6 + k[1][i] / 3 + k[2][i] / 3 + k[3][i] / 6;
    }

    delete[] derivates;
    derivates = nullptr;

    delete[] y_temp;
    y_temp = nullptr;

    for (int i = 0; i < 4; ++i)
    {
        delete[] k[i];
        k[i] = nullptr;
    }
    delete[] k;
    k = nullptr;
}

std::string SolverODEs::getStringMethod()
{
    switch (method_id)
    {
    case Undefined:
        return "Undefined";

    case RungeKutta4:
        return "RungeKutta4";

    default:
        method_id = Undefined;
        return "Undefined";
    }
}
