#include <iostream>
#include <memory>
#include <filesystem>
#include "solutions/RealSolution.h"
#include "solutions/BinarySolution.h"
#include "evaluation/Test1EvaluationFunction.h"
#include "evaluation/Test2EvaluationFunction.h"
#include "GeneticAlgorithm.h"
#include "CSVFile.h"

int main() {
    std::cout << "Lab 04 — Nature Inspired Algorithms: Genetic Algorithm\n";

    std::filesystem::create_directories(RESULTS_DIR);

    const int evals = 10000;
    const int runs = 100;
    const int dim = 10;
    const int population_size = 50;

    auto f1_eval = std::make_shared<Test1EvaluationFunction>();
    auto f2_eval = std::make_shared<Test2EvaluationFunction>(20, 0.2, 6.28);

    std::pair<double,double> f1_domain = {-3,3};
    std::pair<double,double> f2_domain = {-32.768,32.768};

    auto f1_real_start = std::make_shared<RealSolution>(f1_domain);
    auto f1_bin_start  = std::make_shared<BinarySolution>(f1_domain);
    auto f2_real_start = std::make_shared<RealSolution>(f2_domain);
    auto f2_bin_start  = std::make_shared<BinarySolution>(f2_domain);

    struct Experiment {
        std::string name;
        std::shared_ptr<Evaluation> eval;
        std::shared_ptr<Solution> start_sol;
    };

    std::vector<Experiment> experiments = {
        {"F1_real", f1_eval, f1_real_start},
        {"F1_bin",  f1_eval, f1_bin_start},
        {"F2_real", f2_eval, f2_real_start},
        {"F2_bin",  f2_eval, f2_bin_start}
    };

    for (auto &exp : experiments) {
        std::cout << "Running experiment: " << exp.name << std::endl;

        std::filesystem::path output = std::filesystem::path(RESULTS_DIR) / (exp.name + ".csv");
        CSVFile csv_file(output);

        std::vector<double> avg_series (evals, 0.0);

        std::vector<double> last_in_series {};
        last_in_series.reserve(evals);

        for (int r = 0; r < runs; r++) {
            f1_real_start->fit_to_dim(dim);
            f1_bin_start->fit_to_dim(dim);
            f2_real_start->fit_to_dim(dim);
            f2_bin_start->fit_to_dim(dim);

            exp.eval->clear_state();

            GeneticAlgorithm ga(exp.eval, exp.start_sol, population_size);
            auto best = ga.find_solution();

            auto history = exp.eval->get_history();

            std::vector<double> best_series(evals, 0.0);
            double best_so_far = std::numeric_limits<double>::infinity();

            for (int i = 0; i < evals; i++) {
                double val = (i < history.size() ? history[i] : best_so_far);
                best_so_far = std::min(best_so_far, val);
                best_series[i] = best_so_far;

                avg_series[i] += best_so_far;
            }
        }

        for (auto &v : avg_series) {
            v /= runs;
        }

        for (int i = 0; i < evals; i++) {
            csv_file.append_row(i, avg_series[i]);
        }
    }

    std::cout << "All experiments completed. Results saved in " << RESULTS_DIR << std::endl;
    return 0;
}
