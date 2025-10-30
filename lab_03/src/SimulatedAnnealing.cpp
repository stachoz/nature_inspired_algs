//
// Created by dcend on 29.10.2025.
//

#include "SimulatedAnnealing.h"

#include <iostream>

SimulatedAnnealing::SimulatedAnnealing(Evaluation *evaluation, Neighborhood *neighborhood,
                                       double started_temperature): LocalSearch(
                                                                      evaluation, neighborhood),
                                                                    temperature(started_temperature) {
}

Solution *SimulatedAnnealing::find_solution(Solution *starting_solution) {
  Solution *best_solution = starting_solution;
  int iteration = 0;
  do {
    std::cout << "Iteration: " << iteration << "\n";
    Solution *candidate_solution = neighborhood->get_neighbor(best_solution);
    std::cout << "\nnew candidate" << "\n";
    candidate_solution->print();
    if (is_better(candidate_solution, best_solution) || is_hot_enough()) {
      delete best_solution;
      best_solution = candidate_solution;
    } else {
      delete candidate_solution;
    }
    update_temperature();
  } while (is_stopping_condition_met(best_solution) || !is_max_iterations_reached(iteration++));
  return best_solution;
}

bool SimulatedAnnealing::is_stopping_condition_met(Solution *solution) {
  return false;
}

bool SimulatedAnnealing::is_hot_enough() {
  return false; //todo to implement
}

void SimulatedAnnealing::update_temperature() {
  //todo to implement
}
