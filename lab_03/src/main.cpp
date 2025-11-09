#include <iostream>

#include "AlgorithmExperimentRunner.h"
#include "evaluation/Test2EvaluationFunction.h"
#include "neighborhood/DefaultRealNeighborhood.h"
#include "solutions/RealSolution.h"

int main() {
    std::cout << "lab_03 — Nature Inspired Algorithms" << std::endl;

    const AlgorithmExperimentRunner runner;
    runner.run();

    return 0;
}
