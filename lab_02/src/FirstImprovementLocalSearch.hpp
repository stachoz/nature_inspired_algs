//
// Created by dcend on 22.10.2025.
//

#ifndef FIRSTIMPROVEMENTLOCALSEARCH_H
#define FIRSTIMPROVEMENTLOCALSEARCH_H
#include "LocalSearch.hpp"

template<typename T>
class FirstImprovementLocalSearch : public LocalSearch<T> {
public:
  T find_solution(T starting_solution) override;

protected:
  double evaluation_function(T solution) override;

  std::vector<T> neighborhood_function(T solution) override;

  bool is_stopping_condition_met(T solution) override;
};

template<typename T>
T FirstImprovementLocalSearch<T>::find_solution(T starting_solution) {
  T best_solution = starting_solution;
  do {
    std::vector<T> neighborhood = neighborhood_function(best_solution);
    for (int i = 0; i < neighborhood.size(); i++) {
      T neighbor = neighborhood[i];
      if (evaluation_function(neighbor) < evaluation_function(best_solution)) {
        best_solution = neighbor;
      }
    }
  } while (!is_stopping_condition_met(best_solution));
  return best_solution;
}

template<typename T>
double FirstImprovementLocalSearch<T>::evaluation_function(T solution) {
}

template<typename T>
std::vector<T> FirstImprovementLocalSearch<T>::neighborhood_function(T solution) {
}

template<typename T>
bool FirstImprovementLocalSearch<T>::is_stopping_condition_met(T solution) {
}


#endif //FIRSTIMPROVEMENTLOCALSEARCH_H
