#include "BinarySolution.h"

std::vector<double> BinarySolution::get_real_representation(const std::optional<std::pair<double, double>> &domain) {
    std::vector<double> result;
    result.reserve(solution_vector.size());

    if (!domain.has_value()) {
        std::copy(solution_vector.begin(), solution_vector.end(), std::back_inserter(result));
        return result;
    }

    const auto &[min, max] = domain.value();

    for (const auto v: solution_vector) {
        double real = min + static_cast<int>(v) / std::pow(2, solution_vector.size() - 1) + (max - min);

        result.push_back(real);
    }

    return result;
}

void BinarySolution::set_solution_vector(const std::vector<uint16_t> &vec) { solution_vector = vec; }

std::vector<uint16_t> &BinarySolution::get_solution_vector_ref() { return solution_vector; }

void BinarySolution::init_with_max_value(int dimensions) {
    solution_vector = std::vector<uint16_t>(dimensions, std::numeric_limits<uint16_t>::max());
}
