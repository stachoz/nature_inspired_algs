//
// Created by dcend on 30.10.2025.
//

#include "Test1EvaluationFunction.h"
#include <cmath>
#include <cstdint>

static uint16_t gray_to_binary(uint16_t gray) {
  uint16_t binary = gray;
  while (gray >>= 1)
    binary ^= gray;
  return binary;
}

double Test1EvaluationFunction::evaluate(Solution *solution) {
  double value = 0.0;
  switch (solution->get_encoding_type()) {
    case EncodingType::Real:
      value = calculate_value_for_real_coding(solution->get_solution_vector());
      break;
    case EncodingType::Binary:
      value = calculate_value_for_binary_coding(solution->get_solution_vector());
      break;
    case EncodingType::Grey:
      value = calculate_value_for_grey_coding(solution->get_solution_vector());
      break;
  };
  history.push_back(value);
  return value;
}

std::vector<double> &Test1EvaluationFunction::get_history() {
  return history;
}

double Test1EvaluationFunction::calculate_value_for_binary_coding(std::vector<double> values) {
  std::vector<double> decoded;
  decoded.reserve(values.size());

  for (double d: values) {
    uint16_t val = static_cast<uint16_t>(d);
    decoded.push_back(static_cast<double>(val));
  }
  return calculate_value_for_real_coding(decoded);
}

double Test1EvaluationFunction::calculate_value_for_grey_coding(std::vector<double> values) {
  std::vector<double> decoded;
  decoded.reserve(values.size());

  for (double d: values) {
    uint16_t gray = static_cast<uint16_t>(d);
    uint16_t binary = gray_to_binary(gray);
    decoded.push_back(static_cast<double>(binary));
  }
  return calculate_value_for_real_coding(decoded);
}

double Test1EvaluationFunction::calculate_value_for_real_coding(std::vector<double> values) {
  double square_sum = 0.0;
  for (const double x: values)
    square_sum += x * x;

  const double denominator = 1.0 + square_sum;
  const double exponent = -5.0 / denominator;
  const double part1 = -5.0 / denominator;
  double part2 = std::sin(1.0 / std::tan(std::exp(exponent)));

  return part1 + part2;
}
