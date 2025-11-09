#include "DefaultBinaryNeighborhood.h"

#include <iostream>

#include "solutions/BinarySolution.h"

DefaultBinaryNeighborhood::DefaultBinaryNeighborhood(int number_of_bits_to_manipulate, uint16_t min_dimension_value,
                                                     uint16_t max_dimension_value) {
    this->number_of_bits_to_manipulate = number_of_bits_to_manipulate;
    this->min_dimension_value = min_dimension_value;
    this->max_dimension_value = max_dimension_value;
}

std::shared_ptr<Solution> DefaultBinaryNeighborhood::get_neighbor(std::shared_ptr<Solution> solution) {
    auto* binary_solution = dynamic_cast<BinarySolution *>(solution.get());
    auto new_neighbor = std::make_shared<BinarySolution>(*binary_solution);

    std::uniform_int_distribution<int> dimension_distribution(0, binary_solution->get_solution_vector_ref().size() - 1);
    std::uniform_int_distribution<int> bit_distribution(0, number_of_bits_to_manipulate - 1);

    std::vector<uint16_t>& solution_vector = new_neighbor->get_solution_vector_ref();

    // for (int i = 0; i < number_of_bits_to_manipulate; i++) {
    int random_bit = bit_distribution(gen);
    int random_dimension = dimension_distribution(gen);
    solution_vector[random_dimension] ^= 1 << random_bit;
    // }

    return new_neighbor;
}
