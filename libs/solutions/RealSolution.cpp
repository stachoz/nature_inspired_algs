#include "RealSolution.h"

#include <iostream>
#include <ostream>

std::vector<double> RealSolution::get_real_representation() {
    std::vector<double> result{};

    const auto &[min, max] = domain;
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

Solution* RealSolution::clone() const{
    return new RealSolution(*this);
}