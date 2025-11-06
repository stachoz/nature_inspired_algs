#pragma once

#include <complex>
#include <cstdint>
#include <ranges>

#include "Solution.h"

class BinarySolution : public Solution {
public:
    std::vector<double> get_real_representation(const std::optional<std::pair<double, double>> &domain) override;

    void set_solution_vector(const std::vector<uint16_t> &vec);

    std::vector<uint16_t> &get_solution_vector_ref();

    void init_with_value(int dimensions, double value) override;

private:
    std::vector<uint16_t> solution_vector;
};