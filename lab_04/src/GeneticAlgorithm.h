#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include <random>
#include <iostream>
#include "solutions/Solution.h"
#include "evaluation/Evaluation.h"
#include <unordered_map>


// Poprawiony prosty Genetyczny Algorytm
class GeneticAlgorithm {
public:
    GeneticAlgorithm(std::shared_ptr<Evaluation> eval,
                     std::shared_ptr<Solution> start_solution,
                     int population_size = 50,
                     double crossover_prob = 0.9,
                     double mutation_prob = 0.1,
                     int tournament_size = 3,
                     int max_evaluations = 10000)
        : evaluation(eval),
          population_size(population_size),
          crossover_prob(crossover_prob),
          mutation_prob(mutation_prob),
          tournament_size(tournament_size),
          max_evaluations(max_evaluations)
    {
        dimension = start_solution->get_real_representation().size();
        init_population(start_solution);
    }

    std::shared_ptr<Solution> find_solution() {
        evaluate_population(population);

        while (evaluation->get_eval_counter() < max_evaluations) {
            std::vector<std::shared_ptr<Solution>> offspring;

            while (offspring.size() < population_size) {
                auto parent1 = tournament_select();
                auto parent2 = tournament_select();
                auto children = crossover(parent1, parent2);
                offspring.push_back(children.first);
                if (offspring.size() < population_size)
                    offspring.push_back(children.second);
            }

            for (auto &child : offspring)
                mutate(child);

            evaluate_population(offspring);

            // Sukcesja z elitą
            auto best_it = std::max_element(population.begin(), population.end(),
                                            [this](const auto &a, const auto &b) {
                                                return fitness[a.get()] < fitness[b.get()];
                                            });
            auto best_solution = *best_it;

            // Aktualizacja populacji i fitness
            population = offspring;
            population[0] = best_solution; // elita
        }

        auto best_it = std::max_element(population.begin(), population.end(),
                                        [this](const auto &a, const auto &b) {
                                            return fitness[a.get()] < fitness[b.get()];
                                        });
        return *best_it;
    }

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

    void init_population(std::shared_ptr<Solution> start_solution) {
        population.clear();
        fitness.clear();
        for (int i = 0; i < population_size; i++) {
            auto sol = std::shared_ptr<Solution>(start_solution->clone());
            sol->fit_to_dim(dimension);
            randomize_solution(sol);
            population.push_back(sol);
        }
    }

    void randomize_solution(std::shared_ptr<Solution> sol) {
        if (auto real_sol = std::dynamic_pointer_cast<RealSolution>(sol)) {
            auto &vec = real_sol->get_solution_vector_ref();
            auto [min, max] = real_sol->get_domain();
            std::uniform_real_distribution<double> dist(min, max);
            for (auto &x : vec) x = dist(rng);
        } else if (auto bin_sol = std::dynamic_pointer_cast<BinarySolution>(sol)) {
            auto [min, max] = bin_sol->get_domain();
            std::uniform_int_distribution<uint16_t> dist(0, std::numeric_limits<uint16_t>::max());
            for (auto &x : bin_sol->get_solution_vector_ref()) x = dist(rng);
        }
    }

    void evaluate_population(std::vector<std::shared_ptr<Solution>> &pop) {
        for (auto &sol : pop) {
            if (evaluation->get_eval_counter() >= max_evaluations) break;
            double val = evaluation->evaluate(sol.get());
            fitness[sol.get()] = val;

            // Aktualizacja best-so-far w historii
            if (evaluation->get_history().empty() || val > evaluation->get_history().back())
                evaluation->add_to_history(val);
            else
                evaluation->add_to_history(evaluation->get_history().back());
        }
    }

    std::shared_ptr<Solution> tournament_select() {
        std::uniform_int_distribution<int> dist(0, population_size - 1);
        std::shared_ptr<Solution> best = nullptr;
        double best_val = -1e100;

        for (int i = 0; i < tournament_size; i++) {
            auto candidate = population[dist(rng)];
            double val = fitness[candidate.get()];
            if (!best || val > best_val) {
                best = candidate;
                best_val = val;
            }
        }
        return best;
    }

    std::pair<std::shared_ptr<Solution>, std::shared_ptr<Solution>> crossover(
            std::shared_ptr<Solution> parent1,
            std::shared_ptr<Solution> parent2) {

        std::uniform_real_distribution<double> dist(0.0,1.0);
        if (dist(rng) > crossover_prob)
            return {clone_solution(parent1), clone_solution(parent2)};

        auto child1 = clone_solution(parent1);
        auto child2 = clone_solution(parent2);

        if (auto real1 = std::dynamic_pointer_cast<RealSolution>(child1)) {
            auto real2 = std::dynamic_pointer_cast<RealSolution>(child2);
            std::uniform_int_distribution<int> point_dist(1, dimension - 1);
            int point = point_dist(rng);
            auto &v1 = real1->get_solution_vector_ref();
            auto &v2 = real2->get_solution_vector_ref();
            for (int i = point; i < dimension; i++)
                std::swap(v1[i], v2[i]);
        } else if (auto bin1 = std::dynamic_pointer_cast<BinarySolution>(child1)) {
            auto bin2 = std::dynamic_pointer_cast<BinarySolution>(child2);
            std::uniform_int_distribution<int> point_dist(0, dimension - 1);
            int point = point_dist(rng);
            auto &v1 = bin1->get_solution_vector_ref();
            auto &v2 = bin2->get_solution_vector_ref();
            for (int i = point; i < dimension; i++)
                std::swap(v1[i], v2[i]);
        }

        return {child1, child2};
    }

    void mutate(std::shared_ptr<Solution> sol) {
        std::uniform_real_distribution<double> dist(0.0,1.0);

        if (auto real_sol = std::dynamic_pointer_cast<RealSolution>(sol)) {
            auto &vec = real_sol->get_solution_vector_ref();
            auto [min, max] = real_sol->get_domain();
            std::normal_distribution<double> mutation_dist(0.0, 0.1 * (max - min));
            for (auto &x : vec) {
                if (dist(rng) < mutation_prob) {
                    x += mutation_dist(rng);
                    x = std::min(std::max(x, min), max);
                }
            }
        } else if (auto bin_sol = std::dynamic_pointer_cast<BinarySolution>(sol)) {
            auto &vec = bin_sol->get_solution_vector_ref();
            for (auto &x : vec) {
                if (dist(rng) < mutation_prob)
                    x ^= (1 << (rng() % 16));
            }
        }
    }

    std::shared_ptr<Solution> clone_solution(std::shared_ptr<Solution> sol) {
        return std::shared_ptr<Solution>(sol->clone());
    }
};
