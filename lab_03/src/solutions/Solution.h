#pragma once

#include "vector"
#include <optional>

class Solution {
public:
    virtual std::vector<double> get_real_representation(const std::optional<std::pair<double, double>>& domain) = 0;
    virtual void init_with_value(int dimensions, double value) = 0;
    virtual ~Solution() = default;
};

