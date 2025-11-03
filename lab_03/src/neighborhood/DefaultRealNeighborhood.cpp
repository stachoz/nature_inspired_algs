#include "DefaultRealNeighborhood.h"
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
    solution_vector[dimension] = solution_vector[dimension] + normal_distribution(gen);

    return new_neighbor;
}
