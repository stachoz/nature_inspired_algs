#include "SimulatedAnnealing.h"

#include <iostream>
#include <random>

SimulatedAnnealing::SimulatedAnnealing(Evaluation *evaluation, Neighborhood *neighborhood, double started_temperature,
                                       double cooling_rate, int eval_num) :
    LocalSearch(evaluation, neighborhood, eval_num), temperature(started_temperature), cooling_rate(cooling_rate) {}

Solution *SimulatedAnnealing::find_solution(Solution *starting_solution) {
    Solution *best_solution = starting_solution;
    do {
        Solution *candidate_solution = neighborhood->get_neighbor(best_solution);

        if (is_better(candidate_solution, best_solution) || is_hot_enough()) {
            best_solution = candidate_solution;
        }
        update_temperature();
    } while (!is_max_iterations_reached());

    return best_solution;
}

bool SimulatedAnnealing::is_stopping_condition_met(Solution *solution) { return false; }

bool SimulatedAnnealing::is_hot_enough() const {
    return dist(rng) < temperature;
}

void SimulatedAnnealing::update_temperature() { temperature *= cooling_rate; }
