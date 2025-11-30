//
// Created by dcend on 24.11.2025.
//

#include "GeneralizedRosenbrock.h"
double GeneralizedRosenbrock::evaluate(Solution *solution) {
    increase_counter();
    const std::vector<double> x = solution->get_real_representation();
    const int D = x.size();
    double sum = 0.0;
    for (int j = 0; j <= D - 2; j++) {
        double term1 = x[j + 1] - x[j] * x[j];
        double term2 = x[j] - 1.0;
        sum += 100.0 * term1 * term1 + term2 * term2;
    }

    add_to_history(sum);
    return sum;
}
