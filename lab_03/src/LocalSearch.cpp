//
// Created by dcend on 29.10.2025.
//
#include "LocalSearch.h"

LocalSearch::LocalSearch(Evaluation *evaluation, Neighborhood *neighborhood) {
  this->evaluation = evaluation;
  this->neighborhood = neighborhood;
  max_iterations = 10000;
  minimalize = true;
}

LocalSearch::~LocalSearch() {
  delete evaluation;
  delete neighborhood;
}

void LocalSearch::set_evaluation(Evaluation *evaluation) {
  delete this->evaluation;
  this->evaluation = evaluation;
}

void LocalSearch::set_neighborhood(Neighborhood *neighborhood) {
  delete this->neighborhood;
  this->neighborhood = neighborhood;
}

void LocalSearch::set_minimalize(bool minimalize) {
  this->minimalize = minimalize;
}

void LocalSearch::set_max_iterations(int max_iterations) {
  this->max_iterations = max_iterations;
}

bool LocalSearch::is_better(Solution *candidate_solution, Solution *current_best_solution) const {
  if (!minimalize) {
    return evaluation->evaluate(candidate_solution) <= evaluation->evaluate(current_best_solution);
  }
  return evaluation->evaluate(candidate_solution) > evaluation->evaluate(current_best_solution);
}

bool LocalSearch::is_max_iterations_reached(int interation) const {
  return interation >= max_iterations;
}
