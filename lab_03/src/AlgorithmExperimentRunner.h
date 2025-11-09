#pragma once
#include <iostream>
#include <memory>
#include <vector>

#include "CSVFile.h"
#include "LocalSearch.h"
#include "SimulatedAnnealing.h"
#include "neighborhood/Neighborhood.h"
#include "solutions/Solution.h"


class AlgorithmExperimentRunner {
public:
    void run() const;
    void perform(std::unique_ptr<LocalSearch> local_search, std::string_view filename) const;
private:
    int evals = 10000;
    int runs = 100;
    std::vector<int> dimensions {10};
};
