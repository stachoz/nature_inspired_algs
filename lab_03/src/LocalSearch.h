//
// Created by dcend on 28.10.2025.
//

#ifndef ABSTRACTLOCALSEARCHALGORITM_H
#define ABSTRACTLOCALSEARCHALGORITM_H
#include "Solution.h"
#include "Neighborhood.h"
#include "Evaluation.h"

class LocalSearch {
public:
  LocalSearch(Evaluation *evaluation, Neighborhood *neighborhood);

  virtual Solution *find_solution(Solution *starting_solution) = 0;

  virtual ~LocalSearch();

  void set_evaluation(Evaluation *evaluation);

  void set_neighborhood(Neighborhood *neighborhood);

  void set_minimalize(bool minimalize);

  void set_max_iterations(int max_iterations);

protected:
  Evaluation *evaluation;
  Neighborhood *neighborhood;
  bool minimalize;
  int max_iterations;

  bool is_better(Solution *candidate_solution, Solution *current_best_solution) const;

  bool is_max_iterations_reached(int interation) const;

  virtual bool is_stopping_condition_met(Solution *solution) = 0;
};


#endif //ABSTRACTLOCALSEARCHALGORITM_H
