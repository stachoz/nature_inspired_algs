#pragma once

#include "Evaluation.h"

class Test2EvaluationFunction : public Evaluation {
public:
    Test2EvaluationFunction(double a, double b, double c);
    double evaluate(Solution *solution) override;

private:
    double a;
    double b;
    double c;
};
