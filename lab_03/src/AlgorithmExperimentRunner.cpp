#include "AlgorithmExperimentRunner.h"

#include "evaluation/Evaluation.h"
#include "neighborhood/Neighborhood.h"
#include "solutions/RealSolution.h"

AlgorithmExperimentRunner::AlgorithmExperimentRunner(const std::vector<int> &dimensions, int runs, int evals,
                                                     int start_temp, double cooling_rate,
                                                     std::unique_ptr<Evaluation> evaluation,
                                                     std::unique_ptr<Neighborhood> neighborhood,
                                                     std::unique_ptr<Solution> solution,
                                                     double max_dim_val) :  dimensions(dimensions),
                                                                            runs(runs),
                                                                            evals(evals),
                                                                            max_dim_value(max_dim_val),
                                                                            evaluation(std::move(evaluation)),
                                                                            neighborhood(std::move(neighborhood)),
                                                                            start_solution(std::move(solution)) {
    simulated_annealing = std::make_unique<SimulatedAnnealing>(this->evaluation.get(),
        this->neighborhood.get(),start_temp, cooling_rate, evals);
}

void AlgorithmExperimentRunner::run(std::string_view filename) {
    std::vector avg_series(evals, 0.0);

    for (int dim : dimensions) {
        std::filesystem::path output = std::filesystem::path(RESULTS_DIR) / (std::string(filename) + "_" + std::to_string(dim) + ".csv");
        CSVFile csv_file(output);

        start_solution->init_with_max_value(dim);

        for (int i = 0; i < runs; i++) {
            [[maybe_unused]] Solution *best_solution = simulated_annealing->find_solution(start_solution.get());

            auto evaluation_history = evaluation->get_history();

            for (int j = 0; j < evals; j++) {
                avg_series[j] += evaluation_history[j];
            }

            evaluation->clear_state();
        }

        for (double &val: avg_series) {
            val /= runs;
        }

        for (int i = 0; i < evals; i++) {
            csv_file.append_row(i, avg_series[i]);
        }
    }
}
void AlgorithmExperimentRunner::change_evaluation(std::unique_ptr<Evaluation> new_evaluation) {
    evaluation = std::move(new_evaluation);
}

void AlgorithmExperimentRunner::change_neighborhood(std::unique_ptr<Neighborhood> new_neighborhood) {
    neighborhood = std::move(new_neighborhood);
}
