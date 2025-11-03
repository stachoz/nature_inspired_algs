#include "Test1EvaluationFunction.h"
#include "solutions/Solution.h"
#include <cmath>

double Test1EvaluationFunction::evaluate(Solution *solution) {
    increase_counter();

    std::vector<double> real_solution_vec = solution->get_real_representation({});

    double square_sum = 0.0;
    for (const double x : real_solution_vec) {
        square_sum += x * x;
    }

    const double denominator = 1.0 + square_sum;
    const double exponent = -5.0 / denominator;
    const double part1 = -5.0 / denominator;
    const double part2 = std::sin(1.0 / std::tan(std::exp(exponent)));

    const double result = part1 + part2;
    add_to_history(result);

    return result;
}
