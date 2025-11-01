#include "DefaultBinaryNeighborhood.h"
#include "BinarySolution.h"

DefaultBinaryNeighborhood::DefaultBinaryNeighborhood(int number_of_bits_to_manipulate, uint16_t min_dimension_value,
                                                     uint16_t max_dimension_value) {
    this->number_of_bits_to_manipulate = number_of_bits_to_manipulate;
    this->min_dimension_value = min_dimension_value;
    this->max_dimension_value = max_dimension_value;
}

Solution *DefaultBinaryNeighborhood::get_neighbor(Solution *solution) {
    auto* binary_solution = dynamic_cast<BinarySolution *>(solution);
    auto* new_neighbor = new BinarySolution(*binary_solution);

    std::uniform_int_distribution<size_t> dimension_distribution(0, solution->get_dimension() - 1);
    std::uniform_int_distribution<int> bit_distribution(0, 15); // TODO

    std::vector<double> solution_vector = new_neighbor->get_solution_vector();

    for (int i = 0; i < number_of_bits_to_manipulate; i++) {
        uint16_t new_dimension_value = 0;
        size_t dimension = 0;
        do {
            int bit = bit_distribution(gen);
            dimension = dimension_distribution(gen);
            new_dimension_value = static_cast<uint16_t>(solution_vector[dimension]);
            new_dimension_value ^= (1 << bit);
        } while (new_dimension_value < min_dimension_value || new_dimension_value > max_dimension_value);
        solution_vector[dimension] = static_cast<double>(new_dimension_value);
    }

    new_neighbor->set_solution_vector(solution_vector);
    return new_neighbor;
}
