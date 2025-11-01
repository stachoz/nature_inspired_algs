#pragma once

#include <cstddef>
#include "EncodingType.h"
#include "vector"

class Solution {
public:
    Solution(size_t dimension, EncodingType encoding_type);

    virtual ~Solution() = default;

    virtual std::vector<double> get_solution_vector() = 0;

    virtual void set_solution_vector(const std::vector<double>& solution_vector) = 0;

    size_t get_dimension() const;

    EncodingType get_encoding_type() const;

    virtual void print() = 0;

protected:
    size_t dimension;
    EncodingType encoding_type;
};
