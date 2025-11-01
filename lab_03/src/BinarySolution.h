#pragma once

#include <cstdint>
#include "Solution.h"

class BinarySolution : public Solution {
public:
    BinarySolution(size_t dimension, EncodingType encoding_type);

    std::vector<double> get_solution_vector() override;

    void set_solution_vector(const std::vector<double>& solution_vector) override;

    void print() override;

protected:
    std::vector<uint16_t> solution_vector;
};