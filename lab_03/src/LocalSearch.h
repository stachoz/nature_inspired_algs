#pragma once
#include "evaluation/Evaluation.h"
#include "neighborhood/Neighborhood.h"


class LocalSearch {
public:
    LocalSearch(std::shared_ptr<Evaluation> evaluation,
                std::shared_ptr<Neighborhood> neighborhood,
                std::shared_ptr<Solution> solution,
                int max_evaluation_num) : evaluation(std::move(evaluation)),
                                                      neighborhood(std::move(neighborhood)),
                                                      starting_solution(std::move(solution)),
                                                      max_eval(max_evaluation_num)  {};

    virtual std::shared_ptr<Solution> find_solution() = 0;

    virtual ~LocalSearch() = default;

    void set_minimalize(bool minimalize);

    void set_max_eval(int max_iterations);

    Evaluation *get_evaluation() const;

    std::shared_ptr<Solution> get_starting_solution() const {
        return starting_solution;
    }

protected:
    std::shared_ptr<Evaluation> evaluation;
    std::shared_ptr<Neighborhood> neighborhood;
    std::shared_ptr<Solution> starting_solution;

    bool minimalize = true;
    int max_eval = 10'000;

    bool is_better(double candidate_evaluation, double current_best_evaluation) const;

    bool is_max_iterations_reached() const;

    virtual bool is_stopping_condition_met(Solution *solution) = 0;
};