#include "BinarySolution.h"

std::vector<double> BinarySolution::get_real_representation() {
    std::vector<double> result;
    result.reserve(solution_vector.size());

    auto& [min, max] = domain;

    constexpr int bits = 16;
    const double scale = (max - min) / (std::pow(2, bits) - 1);


    for (const auto v: solution_vector) {
        double real = min + static_cast<double>(v) * scale;
        result.push_back(real);
    }

    return result;
}

Solution* BinarySolution::clone() const{
    return new BinarySolution(*this);
}