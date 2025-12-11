#include <algorithm>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <numeric>
#include <string>
#include <vector>

#include "CSVFile.h"
#include "GeneticAlgorithm.h"
#include "evaluation/GeneralizedRosenbrock.h"
#include "evaluation/Salomon.h"
#include "evaluation/Whitley.h"
#include "solutions/RealSolution.h"

const std::vector<int> DIMENSIONS = {5, 15, 30};
const int RUNS = 100;

struct Experiment {
    std::string name;
    std::shared_ptr<Evaluation> eval;
    std::shared_ptr<RealSolution> start_sol;
    std::pair<double, double> domain;
};

GeneticAlgorithm::Alg_params get_params_for_experiment(const Experiment& experiment, int dim) {
    GeneticAlgorithm::Alg_params params {};
    params.max_evaluations = 10000 * dim;

    if (experiment.name == "rosenbrock") {
        params.population_size = 50;
        params.crossover_prob = 0.9;
        params.mutation_prob = 0.1;
        params.tournament_size = 3;
    }
    else if (experiment.name == "salomon") {
        params.population_size = 50;
        params.crossover_prob = 0.9;
        params.mutation_prob = 0.1;
        params.tournament_size = 3;
    }
    else if (experiment.name == "whitley") {
        if (dim == 5) {
            params.population_size = 50;
            params.crossover_prob = 0.9;
            params.mutation_prob = 0.1;
            params.tournament_size = 3;
        }
        else if (dim == 15) {
            params.population_size = 250;
            params.crossover_prob = 0.95;
            params.mutation_prob = 0.07;
            params.tournament_size = 5;
        }
        else if (dim == 30) {
            params.population_size = 400;
            params.crossover_prob = 0.95;
            params.mutation_prob = 0.015;
            params.tournament_size = 10;
        }
    }

    return params;
}

int main() {
    std::cout << "Lab 04 — Nature Inspired Algorithms: Genetic Algorithm\n";

    std::filesystem::create_directories(RESULTS_DIR);

    auto f1_eval = std::make_shared<GeneralizedRosenbrock>();
    std::pair<double, double> f1_domain = {-30, 30};
    auto f1_real_start = std::make_shared<RealSolution>(f1_domain);

    auto f2_eval = std::make_shared<Salomon>();
    std::pair<double, double> f2_domain = {-100, 100};
    auto f2_real_start = std::make_shared<RealSolution>(f2_domain);

    auto f3_eval = std::make_shared<Whitley>();
    std::pair<double, double> f3_domain = {-10.24, 10.24};
    auto f3_real_start = std::make_shared<RealSolution>(f3_domain);

    std::vector<Experiment> experiments_template = {
                                                    {"rosenbrock", f1_eval, f1_real_start, f1_domain},
                                                    // {"salomon", f2_eval, f2_real_start, f2_domain},
                                                    // {"whitley", f3_eval, f3_real_start, f3_domain}
    };

    for (int dim : DIMENSIONS) {
        std::cout << "\n--- Running experiments for dim = " << dim << std::endl;

        for (auto &exp : experiments_template) {
            std::cout << "  Running experiment: " << exp.name << std::endl;

            std::string filename = exp.name + "_dim" + std::to_string(dim) + ".csv";
            std::filesystem::path output = std::filesystem::path(RESULTS_DIR) / filename;
            CSVFile csv_file(output);

            auto exp_params = get_params_for_experiment(exp, dim);

            std::vector<std::vector<double>> raw_data(exp_params.max_evaluations, std::vector<double>(RUNS));

            for (int r = 0; r < RUNS; r++) {
                exp.start_sol->fit_to_dim(dim);
                exp.eval->clear_state();

                GeneticAlgorithm ga(exp.eval, exp.start_sol, exp_params);

                auto best = ga.find_solution();

                auto history = exp.eval->get_history();

                double best_so_far = std::numeric_limits<double>::infinity();

                for (int i = 0; i < exp_params.max_evaluations; i++) {
                    double val = (i < history.size() ? history[i] : best_so_far);

                    best_so_far = std::min(best_so_far, val);

                    raw_data[i][r] = best_so_far;
                }
            }

            for (int i = 0; i < exp_params.max_evaluations; i++) {
                csv_file.append_vector_as_row(raw_data[i]);
            }
        }
    }

    std::cout << "All experiments completed. Results saved in " << RESULTS_DIR << std::endl;
    return 0;
}