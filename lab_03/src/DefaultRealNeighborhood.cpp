#include "DefaultRealNeighborhood.h"

#include "RealSolution.h"

DefaultRealNeighborhood::DefaultRealNeighborhood(double min_dimension_value, double max_dimension_value) {
    this->min_dimension_value = min_dimension_value;
    this->max_dimension_value = max_dimension_value;
}

Solution *DefaultRealNeighborhood::get_neighbor(Solution *solution) {
    auto *real_solution = dynamic_cast<RealSolution *>(solution);
    auto *new_neighbor = new RealSolution(*real_solution);

    std::uniform_int_distribution<size_t> dimension_distribution(0, solution->get_dimension() - 1);
    std::normal_distribution<double> normal_distribution(0, 1);

    std::vector<double> solution_vector = new_neighbor->get_solution_vector();
    double new_dimension_value = 0;
    size_t dimension = 0;
    do {
        dimension = dimension_distribution(gen);
        new_dimension_value = solution_vector[dimension];
        new_dimension_value += normal_distribution(gen);
    } while (new_dimension_value < min_dimension_value || new_dimension_value > max_dimension_value);
    solution_vector[dimension] = new_dimension_value;
    new_neighbor->set_solution_vector(solution_vector);
    return new_neighbor;
}
