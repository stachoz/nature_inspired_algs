#pragma once

#include <random>


#include "LocalSearch.h"
#include "evaluation/Evaluation.h"
#include "neighborhood/Neighborhood.h"

class SimulatedAnnealing : public LocalSearch {
public:
    SimulatedAnnealing(std::shared_ptr<Evaluation> eval, std::shared_ptr<Neighborhood> neighborhood,
                       std::shared_ptr<Solution> start_sol, double started_temperature, double cooling_rate,
                       int eval_num);

    std::shared_ptr<Solution> find_solution() override;

protected:
    bool is_stopping_condition_met(Solution *solution) override;

    void update_temperature();

    bool is_hot_enough(double x, double y) const;

    double temperature;
    double cooling_rate;
private:
    mutable std::mt19937 rng{std::random_device{}()};
    mutable std::uniform_real_distribution<double> dist{0.0, 1.0};
};
