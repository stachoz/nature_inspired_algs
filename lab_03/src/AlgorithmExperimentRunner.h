#pragma once
#include <iostream>
#include <memory>
#include <vector>

#include "CSVFile.h"
#include "DefaultRealNeighborhood.h"
#include "Evaluation.h"
#include "LocalSearch.h"
#include "RealSolution.h"
#include "SimulatedAnnealing.h"

class AlgorithmExperimentRunner {
public:
    AlgorithmExperimentRunner(const std::vector<int> &dimensions, int runs, int evals, int start_temp, int cooling_rate,
                              std::unique_ptr<Evaluation> evaluation,
                              std::unique_ptr<DefaultRealNeighborhood> neighborhood, double max_dim_val) :
        dimensions(dimensions), runs(runs), evals(evals), max_dim_value(max_dim_val),
        evaluation(std::move(evaluation)), neighborhood(std::move(neighborhood)) {
        simulated_annealing = std::make_unique<SimulatedAnnealing>(this->evaluation.get(), this->neighborhood.get(),
                                                                   start_temp, cooling_rate);
    }


    void run(std::string_view filename) {
        std::vector avg_series(evals, 0.0);

        for (int dim: dimensions) {
            CSVFile csv_file(std::string(RESULTS_DIR) + "/" + filename.data() + "_" + std::to_string(dim) + ".csv");

            std::shared_ptr<Solution> start_solution = std::make_shared<RealSolution>(dim, EncodingType::Real);
            start_solution->set_solution_vector(std::vector(dim, max_dim_value));

            for (int i = 0; i < runs; i++) {
                [[maybe_unused]] Solution *best_solution = simulated_annealing->find_solution(start_solution.get());

                auto evaluation_history = evaluation->get_history();

                for (int j = 0; j < evals; j++) {
                    avg_series[j] += evaluation_history[j];
                }

                for (double &val: avg_series) {
                    val /= runs;
                }
                evaluation->clear_history();
            }

            for (int i = 0; i < evals; i++) {
                csv_file.append_row(i, avg_series[i]);
            }
        }
    }

    void change_evaluation(std::unique_ptr<Evaluation> new_evaluation) { evaluation = std::move(new_evaluation); }

    void change_neighborhood(std::unique_ptr<Neighborhood> new_evaluation) { neighborhood = std::move(new_evaluation); }

private:
    std::vector<int> dimensions{};
    int runs;
    int evals;
    double max_dim_value;
    std::unique_ptr<Evaluation> evaluation;
    std::unique_ptr<Neighborhood> neighborhood;
    std::unique_ptr<LocalSearch> simulated_annealing;
};
