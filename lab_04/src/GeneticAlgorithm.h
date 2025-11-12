#pragma once

#include <algorithm>
#include <iostream>
#include <memory>
#include <random>
#include <unordered_map>
#include <vector>
#include "evaluation/Evaluation.h"
#include "solutions/BinarySolution.h"
#include "solutions/Solution.h"

#include "solutions/RealSolution.h"


class GeneticAlgorithm {
public:
    GeneticAlgorithm(std::shared_ptr<Evaluation> eval, std::shared_ptr<Solution> start_solution,
                     int population_size = 50, double crossover_prob = 0.9, double mutation_prob = 0.1,
                     int tournament_size = 3, int max_evaluations = 10000);

    std::shared_ptr<Solution> find_solution();

private:
    std::shared_ptr<Evaluation> evaluation;
    std::vector<std::shared_ptr<Solution>> population;
    std::unordered_map<Solution*, double> fitness;
    int population_size;
    double crossover_prob;
    double mutation_prob;
    int tournament_size;
    int max_evaluations;
    int dimension;

    std::mt19937_64 rng{std::random_device{}()};

    void init_population(std::shared_ptr<Solution> start_solution);

    void randomize_solution(std::shared_ptr<Solution> sol);

    void evaluate_population(std::vector<std::shared_ptr<Solution>> &pop);

    std::shared_ptr<Solution> tournament_select();

    std::pair<std::shared_ptr<Solution>, std::shared_ptr<Solution>> crossover(std::shared_ptr<Solution> parent1,
                                                                              std::shared_ptr<Solution> parent2);

    void mutate(std::shared_ptr<Solution> sol);

    std::shared_ptr<Solution> clone_solution(std::shared_ptr<Solution> sol);
};
