#pragma once

#include "Neighborhood.h"
#include <random>

class DefaultRealNeighborhood : public Neighborhood {
public:
    DefaultRealNeighborhood(double min_dimension_value, double max_dimension_value);

    Solution *get_neighbor(Solution *solution) override;

private:
    double min_dimension_value;
    double max_dimension_value;
    std::mt19937 gen{std::random_device{}()};
};
