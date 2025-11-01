//
// Created by dcend on 30.10.2025.
//

#ifndef TEST2EVALUATIONFUNCTION_H
#define TEST2EVALUATIONFUNCTION_H

#include "Evaluation.h"

class Test2EvaluationFunction : public Evaluation {
public:
  Test2EvaluationFunction(double a, double b, double c);

  double evaluate(Solution *solution) override;

  std::vector<double> &get_history() override;

  void clear_history() override;

private:
  double calculate_value_for_binary_coding(const std::vector<double>& values) const;

  double calculate_value_for_grey_coding(const std::vector<double>& values) const;

  double calculate_value_for_real_coding(const std::vector<double>& values) const;

  std::vector<double> history;

  double a, b, c;
};


#endif //TEST2EVALUATIONFUNCTION_H
