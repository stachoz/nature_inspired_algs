#include "RealSolution.h"

#include <iostream>
#include <ostream>

std::vector<double> RealSolution::get_real_representation(const std::optional<std::pair<double, double>> &domain) {
    if(!domain.has_value()) {
        return solution_vector;
    }

    std::vector<double> result{};

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

    return result;
}

void RealSolution::set_solution_vector(const std::vector<double> &vec) { solution_vector = vec; }

std::vector<double>& RealSolution::get_solution_vector_ref() { return solution_vector; }

void RealSolution::init_with_value(int dimensions, double value) {
    solution_vector = std::vector<double>(dimensions, value);
}
