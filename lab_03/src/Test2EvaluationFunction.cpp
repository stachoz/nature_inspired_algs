//
// Created by dcend on 30.10.2025.
//

#include "Test2EvaluationFunction.h"
#include <cmath>
#include <cstdint>

static uint16_t gray_to_binary(uint16_t gray) {
  uint16_t binary = gray;
  while (gray >>= 1)
    binary ^= gray;
  return binary;
}

Test2EvaluationFunction::Test2EvaluationFunction(double a, double b, double c) {
  this->a = a;
  this->b = b;
  this->c = c;
}


double Test2EvaluationFunction::evaluate(Solution *solution) {
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

std::vector<double> &Test2EvaluationFunction::get_history() {
  return history;
}

void Test2EvaluationFunction::clear_history() {
  history.clear();
}

double Test2EvaluationFunction::calculate_value_for_binary_coding(const std::vector<double> &values) const {
  std::vector<double> decoded;
  decoded.reserve(values.size());

  for (const double d: values) {
    const auto val = static_cast<uint16_t>(d);
    decoded.push_back(static_cast<double>(val));
  }
  return calculate_value_for_real_coding(decoded);
}

double Test2EvaluationFunction::calculate_value_for_grey_coding(const std::vector<double> &values) const {
  std::vector<double> decoded;
  decoded.reserve(values.size());

  for (double d: values) {
    const auto gray = static_cast<uint16_t>(d);
    const uint16_t binary = gray_to_binary(gray);
    decoded.push_back(static_cast<double>(binary));
  }
  return calculate_value_for_real_coding(decoded);
}

double Test2EvaluationFunction::calculate_value_for_real_coding(const std::vector<double> &values) const {
  const size_t d = values.size();
  double sum_sq = 0.0;
  double sum_cos = 0.0;

  for (const double x: values) {
    sum_sq += x * x;
    sum_cos += std::cos(c * x);
  }

  double term1 = -a * std::exp(-b * std::sqrt(sum_sq / d));
  double term2 = -std::exp(sum_cos / d);

  return term1 + term2 + a + std::exp(1.0);
}
