#pragma once
#include <iostream>
#include <memory>
#include <vector>

#include "CSVFile.h"
#include "LocalSearch.h"
#include "SimulatedAnnealing.h"
#include "neighborhood/Neighborhood.h"
#include "solutions/Solution.h"


class Evaluation;
class AlgorithmExperimentRunner {
public:
    AlgorithmExperimentRunner(const std::vector<int> &dimensions, int runs, int evals, int start_temp, double cooling_rate,
                              std::unique_ptr<Evaluation> evaluation,
                              std::unique_ptr<Neighborhood> neighborhood,
                              std::unique_ptr<Solution> solution,
                              double max_dim_val);


    void run(std::string_view filename);

    void change_evaluation(std::unique_ptr<Evaluation> new_evaluation);

    void change_neighborhood(std::unique_ptr<Neighborhood> new_neighborhood);

private:
    std::vector<int> dimensions{};
    int runs;
    int evals;
    double max_dim_value;
    std::unique_ptr<Evaluation> evaluation;
    std::unique_ptr<Neighborhood> neighborhood;
    std::unique_ptr<Solution> start_solution;
    std::unique_ptr<LocalSearch> simulated_annealing;
};
