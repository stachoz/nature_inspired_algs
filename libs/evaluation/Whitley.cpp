//
// Created by dcend on 24.11.2025.
//

#include "Whitley.h"
#include <cmath>
#include <vector>
double Whitley::evaluate(Solution *solution) {
    increase_counter();
    const std::vector<double> x = solution->get_real_representation();
    const int D = x.size();
    double sum = 0.0;
    for (int j = 0; j < D; ++j) {
        for (int k = 0; k < D; ++k) {
            double y = 100.0 * std::pow((x[k] - x[j] * x[j]), 2.0) + std::pow(1.0 - x[j], 2.0);

            sum += (y * y) / 4000.0 - std::cos(y) + 1.0;
        }
    }
    return sum;
}
