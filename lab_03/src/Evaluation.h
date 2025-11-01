#pragma once
#include "Solution.h"

class Evaluation {
public:
    virtual double evaluate(Solution *solution) = 0;
    virtual std::vector<double> &get_history() = 0;
    virtual void clear_history() = 0;

    virtual ~Evaluation() = default;
};

