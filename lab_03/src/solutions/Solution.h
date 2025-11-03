#pragma once

#include "vector"
#include <optional>

class Solution {
public:
    virtual std::vector<double> get_real_representation(const std::optional<std::pair<double, double>>& domain) = 0;
    virtual void init_with_max_value(int dimensions) = 0;
    virtual ~Solution() = default;
};

