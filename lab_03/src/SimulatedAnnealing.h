#pragma once

#include "LocalSearch.h"

class SimulatedAnnealing : public LocalSearch {
public:
    SimulatedAnnealing(Evaluation *evaluation, Neighborhood *neighborhood, double started_temperature,
                       double cooling_rate);

    Solution *find_solution(Solution *starting_solution) override;

protected:
    bool is_stopping_condition_met(Solution *solution) override;

    void update_temperature();

    bool is_hot_enough();

    double temperature;
    double cooling_rate;
};
