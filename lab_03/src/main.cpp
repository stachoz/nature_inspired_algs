#include <iostream>

#include "AlgorithmExperimentRunner.h"
#include "BinarySolution.h"
#include "DefaultBinaryNeighborhood.h"
#include "DefaultRealNeighborhood.h"
#include "RealSolution.h"
#include "SimulatedAnnealing.h"
#include "Test1EvaluationFunction.h"
#include "Test2EvaluationFunction.h"

// void test1ForBinary() {
//   Test1EvaluationFunction *evaluation = new Test1EvaluationFunction();
//   Neighborhood *neighborhood = new DefaultBinaryNeighborhood(3, -3, 3);
//   SimulatedAnnealing *simulated_annealing = new SimulatedAnnealing(evaluation, neighborhood, 10.0);
//   Solution *start_solution = new BinarySolution(10, EncodingType::Binary);
//
//   start_solution->set_solution_vector({3, 3, 3, 3, 3, 3, 3, 3, 3, 3});
//
//   Solution *best_solution = simulated_annealing->find_solution(start_solution);
//
//   auto solution_vector = best_solution->get_solution_vector();
//
//   std::cout << "best found vector" << std::endl;
//   for (size_t i = 0; i < solution_vector.size(); i++) {
//     std::cout << solution_vector[i] << std::endl;
//   }
//
//   std::cout << std::endl << "History of evaluation function:" << std::endl;
//   auto evaluation_history = evaluation->get_history();
//   for (size_t i = 0; i < evaluation_history.size(); i++) {
//     std::cout << "try number " << i << "value " << evaluation_history[i] << std::endl;
//   }
//
//   delete simulated_annealing;
//   delete neighborhood;
//   delete evaluation;
//   delete start_solution;
//   delete best_solution;
// }

// void test1ForReal() {
//     Test1EvaluationFunction *evaluation = new Test1EvaluationFunction();
//     Neighborhood *neighborhood = new DefaultRealNeighborhood(-3, 3);
//     SimulatedAnnealing *simulated_annealing = new SimulatedAnnealing(evaluation, neighborhood, 1.0, 0.95);
//     Solution *start_solution = new RealSolution(10, EncodingType::Real);
//
//     start_solution->set_solution_vector({3, 3, 3, 3, 3, 3, 3, 3, 3, 3});
//
//     Solution *best_solution = simulated_annealing->find_solution(start_solution);
//
//     auto solution_vector = best_solution->get_solution_vector();
//
//     std::cout << "Best found vector" << std::endl;
//     for (size_t i = 0; i < solution_vector.size(); i++) {
//         std::cout << solution_vector[i] << std::endl;
//     }
//
//     std::cout << std::endl << "History of evaluation function:" << std::endl;
//     auto evaluation_history = evaluation->get_history();
//     for (size_t i = 0; i < evaluation_history.size(); i++) {
//         std::cout << "try number " << i << "value " << evaluation_history[i] << std::endl;
//     }
//
//     delete simulated_annealing;
//     delete neighborhood;
//     delete evaluation;
//     delete start_solution;
//     delete best_solution;
// }

int main() {
    std::cout << "lab_03 — Nature Inspired Algorithms" << std::endl;
    AlgorithmExperimentRunner runner {
                                    {10},
                                    1,
                                    10'000,
                                    -3,
                                    3,
                                    std::make_unique<Test2EvaluationFunction>(20,0.2,6.28),
                                    std::make_unique<DefaultRealNeighborhood>(-32.768, 32.768)};

    runner.run("first");

    return 0;
}
