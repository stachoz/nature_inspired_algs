#pragma once

#include <cstdint>
#include <random>

#include "Neighborhood.h"


class DefaultBinaryNeighborhood : public Neighborhood {
public:
    DefaultBinaryNeighborhood(int number_of_bits_to_manipulate, uint16_t min_dimension_value,
                              uint16_t max_dimension_value);

    std::shared_ptr<Solution> get_neighbor(std::shared_ptr<Solution> solution) override;

private:
    int number_of_bits_to_manipulate;
    uint16_t min_dimension_value;
    uint16_t max_dimension_value;
    std::mt19937 gen{std::random_device{}()};
};
