#pragma once
#include "evaluation/Evaluation.h"
#include "neighborhood/Neighborhood.h"


class LocalSearch {
public:
    LocalSearch(Evaluation *evaluation, Neighborhood *neighborhood, int eval_num);

    virtual Solution *find_solution(Solution *starting_solution) = 0;

    virtual ~LocalSearch() = default;

    void set_evaluation(Evaluation *evaluation);

    void set_neighborhood(Neighborhood *neighborhood);

    void set_minimalize(bool minimalize);

    void set_max_eval(int max_iterations);


protected:
    Evaluation *evaluation;
    Neighborhood *neighborhood;
    bool minimalize;
    int max_eval;

    bool is_better(Solution *candidate_solution, Solution *current_best_solution) const;

    bool is_max_iterations_reached() const;

    virtual bool is_stopping_condition_met(Solution *solution) = 0;
};