#include "AlgorithmExperimentRunner.h"

#include "evaluation/Evaluation.h"
#include "evaluation/Test1EvaluationFunction.h"
#include "evaluation/Test2EvaluationFunction.h"
#include "neighborhood/DefaultBinaryNeighborhood.h"
#include "neighborhood/DefaultRealNeighborhood.h"
#include "neighborhood/Neighborhood.h"
#include "solutions/BinarySolution.h"
#include "solutions/RealSolution.h"

void AlgorithmExperimentRunner::run() const {
    std::pair<double, double> test1_domain = {-3, 3};
    std::pair<double, double> test2_domain = {-32.768, 32.768};

    std::shared_ptr<Evaluation> test1_eval = std::make_shared<Test1EvaluationFunction>();
    std::shared_ptr<Evaluation> test2_eval = std::make_shared<Test2EvaluationFunction>(20, 0.2, 6.28);

    std::shared_ptr<Neighborhood> bin_ngh1 = std::make_shared<DefaultBinaryNeighborhood>(16, test1_domain.first, test1_domain.second);
    std::shared_ptr<Neighborhood> real_ngh1 = std::make_shared<DefaultRealNeighborhood>(test1_domain.first, test1_domain.second);

    std::shared_ptr<Neighborhood> bin_ngh2 = std::make_shared<DefaultBinaryNeighborhood>(16, test2_domain.first, test2_domain.second);
    std::shared_ptr<Neighborhood> real_ngh2 = std::make_shared<DefaultRealNeighborhood>(test2_domain.first, test2_domain.second);

    std::shared_ptr<Solution> start_bin_sol = std::make_shared<BinarySolution>(test1_domain);
    std::shared_ptr<Solution> start_real_sol = std::make_shared<RealSolution>(test1_domain);

    std::shared_ptr<Solution> start_bin_sol2 = std::make_shared<BinarySolution>(test2_domain);
    std::shared_ptr<Solution> start_real_sol2 = std::make_shared<RealSolution>(test2_domain);

    std::unique_ptr<LocalSearch> ls_test1_real = std::make_unique<SimulatedAnnealing>(
            test1_eval,
            real_ngh1,
            start_real_sol,
            100,
            0.98,
            10'000);

    perform(std::move(ls_test1_real), "test1-real");

    std::unique_ptr<LocalSearch> ls_test1_bin = std::make_unique<SimulatedAnnealing>(
            test1_eval,
            bin_ngh1,
            start_bin_sol,
            100,
            0.98,
            10'000);

    perform(std::move(ls_test1_bin), "test1-bin");

    std::unique_ptr<LocalSearch> ls_test2_real = std::make_unique<SimulatedAnnealing>(
            test2_eval,
            real_ngh2,
            start_real_sol2,
            100,
            0.98,
            10'000);

    perform(std::move(ls_test2_real), "test2-real");

    std::unique_ptr<LocalSearch> ls_test2_bin = std::make_unique<SimulatedAnnealing>(
            test2_eval,
            bin_ngh2,
            start_bin_sol2,
            700,
            0.98,
            10'000);

    perform(std::move(ls_test2_bin), "test2-bin");
}

void AlgorithmExperimentRunner::perform(std::unique_ptr<LocalSearch> local_search, std::string_view filename) const {

    for (int dim: dimensions) {
        std::vector avg_series(evals, 0.0);

        std::filesystem::path output =
                std::filesystem::path(RESULTS_DIR) / (std::string(filename) + "_" + std::to_string(dim) + ".csv");
        CSVFile csv_file(output);

        local_search->get_starting_solution()->fit_to_dim(dim);

        for (int i = 0; i < runs; i++) {
            [[maybe_unused]] auto best_solution = local_search->find_solution();

            auto evaluation_history = local_search->get_evaluation()->get_history();

            for (int j = 0; j < evals; j++) {
                avg_series[j] += evaluation_history[j];
            }

            local_search->get_evaluation()->clear_state();
        }

        for (double &val: avg_series) {
            val /= runs;
        }

        for (int i = 0; i < evals; i++) {
            csv_file.append_row(i, avg_series[i]);
        }
    }
}
