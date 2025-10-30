//
// Created by dcend on 29.10.2025.
//

#include "RealSolution.h"


RealSolution::RealSolution(size_t dimension, EncodingType encoding_type): Solution(dimension, encoding_type) {
}

void RealSolution::set_solution_vector(std::vector<double> solution_vector) {
  dimension = solution_vector.size();
  this->solution_vector.resize(dimension);
  for (int i = 0; i < dimension; i++) {
    this->solution_vector[i] = solution_vector[i];
  }
}


std::vector<double> RealSolution::get_solution_vector() {
  return solution_vector;
}

RealSolution &RealSolution::operator=(const RealSolution &other) {
  if (this != &other) {
    Solution::operator=(other);
    solution_vector.resize(dimension);
    for (size_t i = 0; i < dimension; i++) {
      solution_vector[i] = other.solution_vector[i];
    }
  }
  return *this;
}
