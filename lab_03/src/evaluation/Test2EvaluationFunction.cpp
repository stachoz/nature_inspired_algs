#include "Test2EvaluationFunction.h"
#include <cmath>
#include <iostream>
#include <ostream>

Test2EvaluationFunction::Test2EvaluationFunction(double a, double b, double c) {
    this->a = a;
    this->b = b;
    this->c = c;
}

double Test2EvaluationFunction::evaluate(Solution *solution) {
    increase_counter();

    // FIXME passing domain
    std::vector<double> real_solution_vec = solution->get_real_representation({{-32.768, 32.768}});

    const size_t d = real_solution_vec.size();
    double sum_sq = 0.0;
    double sum_cos = 0.0;

    for (const double x: real_solution_vec) {
        sum_sq += x * x;
        sum_cos += std::cos(c * x);
    }

    const double term1 = -a * std::exp(-b * std::sqrt(sum_sq / d));
    const double term2 = -std::exp(sum_cos / d);

    const double result = term1 + term2 + a + std::exp(1.0);

    add_to_history(result);

    return result;
}
