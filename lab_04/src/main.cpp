#include <filesystem>
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>

#include "CSVFile.h"
#include "GeneticAlgorithm.h"
#include "evaluation/GeneralizedRosenbrock.h"
#include "evaluation/Salomon.h"
#include "evaluation/Whitley.h"
#include "solutions/RealSolution.h"


const std::vector<int> DIMENSIONS = {5, 15, 30};
const int RUNS = 100;
const int BASE_EVALS = 10000;
const int POPULATION_SIZE = 50;

struct Experiment {
    std::string name;
    std::shared_ptr<Evaluation> eval;
    std::shared_ptr<RealSolution> start_sol;
    std::pair<double, double> domain;
};

int main() {
    std::cout << "Lab 04 — Nature Inspired Algorithms: Genetic Algorithm\n";

    std::filesystem::create_directories(RESULTS_DIR);

    auto f1_eval = std::make_shared<GeneralizedRosenbrock>();
    std::pair<double, double> f1_domain = {-30, 30}; // [cite: 66]
    auto f1_real_start = std::make_shared<RealSolution>(f1_domain);

    auto f2_eval = std::make_shared<Salomon>();
    std::pair<double, double> f2_domain = {-100, 100}; // [cite: 67]
    auto f2_real_start = std::make_shared<RealSolution>(f2_domain);

    auto f3_eval = std::make_shared<Whitley>();
    std::pair<double, double> f3_domain = {-10.24, 10.24}; // [cite: 68]
    auto f3_real_start = std::make_shared<RealSolution>(f3_domain);

    std::vector<Experiment> experiments_template = {
        {"A.1.3_Rosenbrock", f1_eval, f1_real_start, f1_domain},
        // {"A.2.4_Salomon", f2_eval, f2_real_start, f2_domain},
        // {"A.2.5_Whitley", f3_eval, f3_real_start, f3_domain}
    };

    for (int dim : DIMENSIONS) {
        int evals = BASE_EVALS * dim;

        std::cout << "\n--- Running experiments for dim = " << dim << " (MaksF = " << evals << ") ---\n";

        for (auto &exp : experiments_template) {
            std::cout << "  Running experiment: " << exp.name << std::endl;

            std::string filename = exp.name + "_dim" + std::to_string(dim) + ".csv";
            std::filesystem::path output = std::filesystem::path(RESULTS_DIR) / filename;
            CSVFile csv_file(output);

            std::vector<std::vector<double>> raw_data(evals, std::vector<double>(RUNS));

            for (int r = 0; r < RUNS; r++) {
                exp.start_sol->fit_to_dim(dim);

                exp.eval->clear_state();

                GeneticAlgorithm ga(exp.eval, exp.start_sol, POPULATION_SIZE, evals);
                auto best = ga.find_solution();
                auto history = exp.eval->get_history();

                double best_so_far = std::numeric_limits<double>::infinity();

                for (int i = 0; i < evals; i++) {
                    double val = (i < history.size() ? history[i] : best_so_far);

                    best_so_far = std::min(best_so_far, val);

                    raw_data[i][r] = best_so_far;
                }
            }

            for (int i = 0; i < evals; i++) {
                csv_file.append_vector_as_row(raw_data[i]);
            }
        }
    }

    std::cout << "All experiments completed. Results saved in " << RESULTS_DIR << std::endl;
    return 0;
}