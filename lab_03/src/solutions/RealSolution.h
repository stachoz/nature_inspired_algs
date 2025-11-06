#pragma once
#include <bits/stl_algo.h>
#include <limits>
#include "Solution.h"

class RealSolution : public Solution {
public:
    std::vector<double> get_real_representation(const std::optional<std::pair<double, double>> &domain) override;

    void set_solution_vector(const std::vector<double> &vec);

    std::vector<double>& get_solution_vector_ref();

    void init_with_value(int dimensions, double value) override;

private:
    std::vector<double> solution_vector;
};
