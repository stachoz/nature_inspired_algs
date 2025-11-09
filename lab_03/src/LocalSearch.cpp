#include "LocalSearch.h"

#include <iostream>

void LocalSearch::set_minimalize(bool minimalize) { this->minimalize = minimalize; }

void LocalSearch::set_max_eval(int max_iterations) { this->max_eval = max_iterations; }

Evaluation *LocalSearch::get_evaluation() const { return evaluation.get(); }

bool LocalSearch::is_better(double candidate_evaluation, double current_best_evaluation) const {
    const bool result = candidate_evaluation < current_best_evaluation;
    return minimalize ? result : !result;
}

bool LocalSearch::is_max_iterations_reached() const {
    std::cout << evaluation->get_eval_counter() << std::endl;
    return evaluation->get_eval_counter() >= max_eval;
}
