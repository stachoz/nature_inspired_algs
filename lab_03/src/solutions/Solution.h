#pragma once

#include "vector"
#include <optional>

class Solution {
public:
    virtual std::vector<double> get_real_representation() = 0;
    virtual void fit_to_dim(int dim) = 0;
    virtual ~Solution() = default;
};

