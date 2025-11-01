#pragma once
#include "Solution.h"


class RealSolution : public Solution {
public:
    RealSolution(size_t dimension, EncodingType encoding_type);

    std::vector<double> get_solution_vector() override;

    void set_solution_vector(const std::vector<double> &solution_vector) override;

    RealSolution &operator=(const RealSolution &other);

    RealSolution(const RealSolution &other);

    void print() override;

protected:
    std::vector<double> solution_vector;
};
