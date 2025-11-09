#include "SimulatedAnnealing.h"

#include <iostream>
#include <random>

SimulatedAnnealing::SimulatedAnnealing(std::shared_ptr<Evaluation> eval, std::shared_ptr<Neighborhood> neighborhood,
                                       std::shared_ptr<Solution> start_sol, double started_temperature,
                                       double cooling_rate, int eval_num) :
    LocalSearch(std::move(eval), std::move(neighborhood), std::move(start_sol), eval_num), temperature(started_temperature),
    cooling_rate(cooling_rate) {}

std::shared_ptr<Solution> SimulatedAnnealing::find_solution() {
    auto best_solution = starting_solution;
    double best_evaluation = evaluation->evaluate(best_solution.get());
    do {
        auto candidate_solution = neighborhood->get_neighbor(best_solution);
        double candidate_evaluation = evaluation->evaluate(candidate_solution.get());

        if (is_better(candidate_evaluation, best_evaluation) || is_hot_enough(best_evaluation, candidate_evaluation)) {
            best_solution = candidate_solution;
            best_evaluation = candidate_evaluation;
        }

        update_temperature();
    } while (!is_max_iterations_reached());

    return best_solution;
}

bool SimulatedAnnealing::is_stopping_condition_met(Solution *solution) { return false; }

bool SimulatedAnnealing::is_hot_enough(double x, double y) const {
    return dist(rng) < std::exp((x - y) / temperature);
}

void SimulatedAnnealing::update_temperature() { temperature *= cooling_rate; }
