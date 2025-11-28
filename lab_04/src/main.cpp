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
const int BASE_EVALS = 10000;
const int POPULATION_SIZE = 100;

struct Experiment {
    std::string name;
    std::shared_ptr<Evaluation> eval;
    std::shared_ptr<RealSolution> start_sol;
    std::pair<double, double> domain;
};

struct RunResult {
    std::string header_name;
    std::vector<double> generation_history;
};

std::vector<double> generate_log_thresholds(double start_exp, double end_exp, double step) {
    std::vector<double> thresholds;
    for (double e = start_exp; e <= end_exp + 0.001; e += step) {
        thresholds.push_back(std::pow(10.0, e));
    }
    return thresholds;
}

int main() {
    std::cout << "Lab 04 — Nature Inspired Algorithms: Genetic Algorithm\n";

    if (!std::filesystem::exists(RESULTS_DIR)) {
        std::filesystem::create_directories(RESULTS_DIR);
    }

    std::vector<double> QUALITY_THRESHOLDS = generate_log_thresholds(-8.0, 2.0, 0.2);

    auto f1_eval = std::make_shared<GeneralizedRosenbrock>();
    std::pair<double, double> f1_domain = {-30, 30};
    auto f1_real_start = std::make_shared<RealSolution>(f1_domain);

    auto f2_eval = std::make_shared<Salomon>();
    std::pair<double, double> f2_domain = {-100, 100};
    auto f2_real_start = std::make_shared<RealSolution>(f2_domain);

    auto f3_eval = std::make_shared<Whitley>();
    std::pair<double, double> f3_domain = {-10.24, 10.24};
    auto f3_real_start = std::make_shared<RealSolution>(f3_domain);

    std::vector<Experiment> experiments_template = {{"rosenbrock", f1_eval, f1_real_start, f1_domain},
                                                    {"salomon", f2_eval, f2_real_start, f2_domain},
                                                    {"whitley", f3_eval, f3_real_start, f3_domain}};

    for (int dim: DIMENSIONS) {
        int evals = BASE_EVALS * dim;
        std::cout << "\n--- Running experiments for dim = " << dim << " (MaxF = " << evals << ") ---\n";

        std::vector<RunResult> dim_results;

        for (auto &exp: experiments_template) {
            std::cout << "  Function: " << exp.name << "..." << std::endl;

            for (int r = 0; r < RUNS; r++) {
                exp.start_sol->fit_to_dim(dim);
                exp.eval->clear_state();

                GeneticAlgorithm ga(exp.eval, exp.start_sol, POPULATION_SIZE, evals);
                ga.find_solution();

                auto history_gens = exp.eval->get_history();

                double best_so_far = std::numeric_limits<double>::infinity();
                for (size_t i = 0; i < history_gens.size(); i++) {
                    if (history_gens[i] < best_so_far)
                        best_so_far = history_gens[i];
                    history_gens[i] = best_so_far;
                }

                std::string col_name = exp.name + "_run_" + std::to_string(r + 1);
                dim_results.push_back({col_name, history_gens});
            }
        }

        {
            std::string raw_filename = "Wyniki_dim" + std::to_string(dim) + "_RAW.csv";
            std::ofstream raw_file(std::filesystem::path(RESULTS_DIR) / raw_filename);

            raw_file << std::fixed << std::setprecision(20);

            raw_file << "Generacja";
            for (const auto &run: dim_results) {
                raw_file << "," << run.header_name;
            }
            raw_file << "\n";

            size_t max_gens = 0;
            for (const auto &run: dim_results)
                max_gens = std::max(max_gens, run.generation_history.size());

            for (size_t g = 0; g < max_gens; g++) {
                raw_file << (g + 1);
                for (const auto &run: dim_results) {
                    double val;
                    if (g < run.generation_history.size()) {
                        val = run.generation_history[g];
                    } else {
                        val = run.generation_history.back();
                    }
                    raw_file << "," << val;
                }
                raw_file << "\n";
            }
            raw_file.close();
            std::cout << "  Saved RAW: " << raw_filename << std::endl;
        }

        {
            std::string ecdf_filename = "Wyniki_dim" + std::to_string(dim) + "_ECDF.csv";
            std::ofstream ecdf_file(std::filesystem::path(RESULTS_DIR) / ecdf_filename);

            ecdf_file << std::fixed << std::setprecision(20);

            ecdf_file << "Budżet";
            for (double th: QUALITY_THRESHOLDS)
                ecdf_file << "," << th;
            ecdf_file << "\n";

            int total_runs = dim_results.size();
            int step = 50;

            for (int i = 0; i < evals; i += step) {
                ecdf_file << (i + 1);

                int gen_idx = i / POPULATION_SIZE;

                for (double threshold: QUALITY_THRESHOLDS) {
                    int success_count = 0;
                    for (const auto &run: dim_results) {
                        double val;
                        if (gen_idx < run.generation_history.size()) {
                            val = run.generation_history[gen_idx];
                        } else {
                            val = run.generation_history.back();
                        }

                        if (val <= threshold) {
                            success_count++;
                        }
                    }
                    double ratio = static_cast<double>(success_count) / total_runs;
                    ecdf_file << "," << ratio;
                }
                ecdf_file << "\n";
            }
            ecdf_file.close();
            std::cout << "  Saved ECDF: " << ecdf_filename << std::endl;
        }
    }

    return 0;
}
