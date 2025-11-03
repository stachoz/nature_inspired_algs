#include "RealSolution.h"

#include <iostream>
#include <ostream>

std::vector<double> RealSolution::get_real_representation(const std::optional<std::pair<double, double>> &domain) {
    std::vector<double> result{};

    if (domain.has_value()) {
        const auto &[min, max] = domain.value();
        std::for_each(std::begin(solution_vector), std::end(solution_vector), [&result, min, max](double val) {
            if (val >= max) {
                result.push_back(max);
            } else if (val < min) {
                result.push_back(min);
            } else {
                result.push_back(val);
            }
        });
    }

    return result;
}

void RealSolution::set_solution_vector(const std::vector<double> &vec) { solution_vector = vec; }

std::vector<double>& RealSolution::get_solution_vector_ref() { return solution_vector; }

void RealSolution::init_with_max_value(int dimensions) {
    solution_vector = std::vector<double>(dimensions, std::numeric_limits<double>::max());
}
