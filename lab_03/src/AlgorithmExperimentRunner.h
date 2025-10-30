#pragma once
#include <iostream>
#include <memory>
#include <vector>

#include "CSVFile.h"
#include "DefaultRealNeighborhood.h"
#include "LocalSearch.h"
#include "RealSolution.h"
#include "SimulatedAnnealing.h"
#include "Evaluation.h"

class AlgorithmExperimentRunner {
public:
  AlgorithmExperimentRunner(const std::vector<int> &dimensions, int runs, int evals, int start_temp, int cooling_rate,
                            std::unique_ptr<Evaluation> evaluation,
                            std::unique_ptr<DefaultRealNeighborhood> neighborhood) : dimensions(dimensions),
    runs(runs),
    evals(evals),
    evaluation(std::move(evaluation)),
    neighborhood(std::move(neighborhood)) {
    simulated_annealing = std::make_unique<SimulatedAnnealing>(
      this->evaluation.get(),
      this->neighborhood.get(),
      start_temp,
      cooling_rate);
  }


  void run(std::string_view filename) {
    std::vector avg_series(evals, 0.0);

    for (int dim: dimensions) {
      CSVFile csv_file(std::string(RESULTS_DIR) + "/" + filename.data() + "_" + std::to_string(dim) + ".csv");

      std::shared_ptr<Solution> start_solution = std::make_shared<RealSolution>(dim, EncodingType::Real);
      start_solution->set_solution_vector(std::vector(dim, 32.768));

      for (int i = 0; i < runs; i++) {
        [[maybe_unused]] Solution *best_solution = simulated_annealing->find_solution(start_solution.get());

        auto evaluation_history = evaluation->get_history();

        std::clog << evaluation_history.size() << std::endl;

        for (int j = 0; j < evals; j++) {
          avg_series[j] += evaluation_history[j];
        }

        for (double &val: avg_series) {
          val /= runs;
        }
        evaluation->clear_hitory();
      }

      for (int i = 0; i < evals; i++) {
        csv_file.append_row(i, avg_series[i]);
      }
    }
  }

  void change_evaluation(std::unique_ptr<Evaluation> new_evaluation) {
    evaluation = std::move(new_evaluation);
  }

private:
  std::vector<int> dimensions{};
  int runs;
  int evals;
  std::unique_ptr<Evaluation> evaluation;
  std::unique_ptr<Neighborhood> neighborhood;
  std::unique_ptr<SimulatedAnnealing> simulated_annealing;
};
