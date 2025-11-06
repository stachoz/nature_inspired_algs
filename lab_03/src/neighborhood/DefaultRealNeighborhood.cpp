#include "DefaultRealNeighborhood.h"

#include <iostream>
#include <mutex>

#include "solutions/RealSolution.h"

DefaultRealNeighborhood::DefaultRealNeighborhood(double min_dimension_value, double max_dimension_value) {
    this->min_dimension_value = min_dimension_value;
    this->max_dimension_value = max_dimension_value;
}

Solution *DefaultRealNeighborhood::get_neighbor(Solution *solution) {
    auto *real_solution = dynamic_cast<RealSolution *>(solution);
    auto *new_neighbor = new RealSolution(*real_solution);

    std::uniform_int_distribution<size_t> dimension_distribution(0, real_solution->get_solution_vector_ref().size() - 1);
    std::normal_distribution<double> normal_distribution(0, 1);

    std::vector<double>& solution_vector = new_neighbor->get_solution_vector_ref();
    size_t dimension = 0;

    dimension = dimension_distribution(gen);

    double new_dimension_value = solution_vector[dimension] + normal_distribution(gen);

    if (new_dimension_value > max_dimension_value) {
        double excess = new_dimension_value - max_dimension_value;
        new_dimension_value = max_dimension_value - excess;
    }
    else if (new_dimension_value < min_dimension_value) {
        double excess = min_dimension_value - new_dimension_value;
        new_dimension_value = min_dimension_value + excess;
    }


    solution_vector[dimension] = new_dimension_value;

    return new_neighbor;
}
