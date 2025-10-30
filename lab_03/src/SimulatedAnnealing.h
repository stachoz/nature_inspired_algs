//
// Created by dcend on 29.10.2025.
//

#ifndef SIMULATEDANNEALING_H
#define SIMULATEDANNEALING_H
#include  "LocalSearch.h"

class SimulatedAnnealing : public LocalSearch {
public:
  SimulatedAnnealing(Evaluation *evaluation, Neighborhood *neighborhood, double started_temperature);

  Solution* find_solution(Solution* starting_solution) override;

protected:
  bool is_stopping_condition_met(Solution* solution) override;

  void update_temperature();

  bool is_hot_enough();

  double temperature;
};


#endif //SIMULATEDANNEALING_H
