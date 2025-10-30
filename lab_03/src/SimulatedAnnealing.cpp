//
// Created by dcend on 29.10.2025.
//

#include "SimulatedAnnealing.h"

#include <iostream>
#include <random>

SimulatedAnnealing::SimulatedAnnealing(Evaluation *evaluation, Neighborhood *neighborhood, double started_temperature,
                                       double cooling_rate) : LocalSearch(evaluation, neighborhood),
                                                              temperature(started_temperature),
                                                              cooling_rate(cooling_rate) {
}

Solution *SimulatedAnnealing::find_solution(Solution *starting_solution) {
  Solution *best_solution = starting_solution;
  int iteration = 0;
  do {
    std::cout << "iteration " << iteration << std::endl;
    Solution *candidate_solution = neighborhood->get_neighbor(best_solution);
    candidate_solution->print();
    if (is_better(candidate_solution, best_solution) || is_hot_enough()) {
      // delete best_solution;
      best_solution = candidate_solution;
    } else {
      // delete candidate_solution;
    }
    update_temperature();
  } while (is_stopping_condition_met(best_solution) || !is_max_iterations_reached(iteration++));
  return best_solution;
}

bool SimulatedAnnealing::is_stopping_condition_met(Solution *solution) { return false; }

bool SimulatedAnnealing::is_hot_enough() {
  std::mt19937 rng(std::random_device{}());
  std::uniform_real_distribution<double> dist(0.0, 1.0);
  double probability = temperature;
  return dist(rng) < probability;
}

void SimulatedAnnealing::update_temperature() { temperature *= cooling_rate; }
