//
// Created by dcend on 24.11.2025.
//

#include "Salomon.h"
#include <cmath>

double Salomon::evaluate(Solution *solution) {
    increase_counter();
    std::vector<double> x = solution->get_real_representation();
    double sum_sq = 0.0;
    for (const double v: x)
        sum_sq += v * v;
    const double r = std::sqrt(sum_sq);
    const double result = 1.0 - std::cos(2.0 * M_PI * r) + 0.1 * r;
    return result;
}
