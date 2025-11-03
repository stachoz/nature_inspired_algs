#include <iostream>

#include "AlgorithmExperimentRunner.h"
#include "evaluation/Test2EvaluationFunction.h"
#include "neighborhood/DefaultRealNeighborhood.h"
#include "solutions/RealSolution.h"

int main() {
    std::cout << "lab_03 — Nature Inspired Algorithms" << std::endl;
    AlgorithmExperimentRunner runner2{{10},
                                      100,
                                      10'000,
                                      3,
                                      0.95,
                                      std::make_unique<Test2EvaluationFunction>(20, 0.2, 6.28),
                                      std::make_unique<DefaultRealNeighborhood>(-32.768, 32.768),
                                      std::make_unique<RealSolution>(),
                                      32.768};

    runner2.run("test2-real");

    return 0;
}
