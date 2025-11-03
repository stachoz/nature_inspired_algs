#pragma once

#include "Evaluation.h"

class Test1EvaluationFunction : public Evaluation {
public:
    double evaluate(Solution *solution) override;
};