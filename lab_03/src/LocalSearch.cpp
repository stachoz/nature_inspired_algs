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

bool LocalSearch::is_better(Solution *candidate_solution, Solution *current_best_solution) const {
    double candidate_eval = evaluation->evaluate(candidate_solution);
    auto last_value = evaluation->get_last_eval_value();

    if(last_value.has_value()) {
        if(!minimalize) {
            return candidate_eval <= last_value.value();
        }
        return candidate_eval >= last_value.value();
    }

    return true;
}

bool LocalSearch::is_max_iterations_reached() const {
    std::cout << evaluation->get_eval_counter() << std::endl;
    return evaluation->get_eval_counter() >= max_eval;
}
