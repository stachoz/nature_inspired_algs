//
// Created by dcend on 29.10.2025.
//

#include "BinarySolution.h"
#include <iostream>

BinarySolution::BinarySolution(size_t dimension, EncodingType encoding_type): Solution(dimension, encoding_type) {
  solution_vector.resize(dimension);
}

std::vector<double> BinarySolution::get_solution_vector() {
  std::vector<double> result(dimension);
  for (size_t i = 0; i < dimension; i++) {
    result[i] = static_cast<double>(solution_vector[i]);
  }
  return result;
}

void BinarySolution::set_solution_vector(std::vector<double> solution_vector) {
  dimension = solution_vector.size();
  this->solution_vector.resize(dimension);
  for (size_t i = 0; i < dimension; i++) {
    this->solution_vector[i] = static_cast<uint16_t>(solution_vector[i]);
  }
}

BinarySolution &BinarySolution::operator=(const BinarySolution &other) {
  if (this != &other) {
    Solution::operator=(other);
    solution_vector.resize(dimension);
    for (size_t i = 0; i < dimension; i++) {
      solution_vector[i] = other.solution_vector[i];
    }
  }
  return *this;
}

BinarySolution::BinarySolution(const BinarySolution &other): Solution(other) {
  BinarySolution::operator=(other);
}

void BinarySolution::print() {
  std::cout << "BinarySolution" << std::endl;
  std::cout << "dimension: " << dimension << std::endl;
  std::cout << "solution_vector: " << std::endl;
  for (int i = 0; i < dimension; i++) {
    std::cout << solution_vector[i] << std::endl;
  }
}
