#pragma once

#include <iostream>
#include <string>

#include <functional>

class SolverODEs
{
public:
    enum Method
    {
        Undefined,
        RungeKutta4
    };

public:
    SolverODEs();
    virtual ~SolverODEs();

    void SolveRK4(const float* y_in, std::function<void(const float*, float*)>& func, float* y_out, const unsigned int size);

    void setStep(float p_step) { step = p_step; }

    Method getMethod() const { return method_id; }
    std::string getStringMethod();
    void setMethod(Method method) { method_id = method; }

protected:
    
    float step;
    Method method_id;
};
