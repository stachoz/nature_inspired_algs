#include "LocalSearch.h"

#include <iostream>

LocalSearch::LocalSearch(Evaluation *evaluation, Neighborhood *neighborhood, int eval_num) {
    this->evaluation = evaluation;
    this->neighborhood = neighborhood;
    max_eval = eval_num;
    minimalize = false;
}

void LocalSearch::set_evaluation(Evaluation *evaluation) {
    delete this->evaluation;
    this->evaluation = evaluation;
}

void LocalSearch::set_neighborhood(Neighborhood *neighborhood) {
    delete this->neighborhood;
    this->neighborhood = neighborhood;
}

void LocalSearch::set_minimalize(bool minimalize) { this->minimalize = minimalize; }

void LocalSearch::set_max_eval(int max_iterations) { this->max_eval = max_iterations; }


// FIXME evaluation function masn't be used here.
bool LocalSearch::is_better(Solution *candidate_solution, Solution *current_best_solution) const {
    if (!minimalize) {
        return evaluation->evaluate(candidate_solution) <= evaluation->evaluate(current_best_solution);
    }
    return evaluation->evaluate(candidate_solution) > evaluation->evaluate(current_best_solution);
}

bool LocalSearch::is_max_iterations_reached() const {
    std::cout << evaluation->get_eval_counter() << std::endl;
    return evaluation->get_eval_counter() >= max_eval;
}
