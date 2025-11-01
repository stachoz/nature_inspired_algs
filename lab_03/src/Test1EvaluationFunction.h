//
// Created by dcend on 30.10.2025.
//

#ifndef TEST1EVALUATIONFUNCTION_H
#define TEST1EVALUATIONFUNCTION_H
#include "Evaluation.h"


class Test1EvaluationFunction : public Evaluation {
public:
  double evaluate(Solution *solution) override;

  std::vector<double> &get_history() override;

  void clear_history() override;

private:
  static double calculate_value_for_binary_coding(const std::vector<double>& values);

  static double calculate_value_for_grey_coding(const std::vector<double>& values);

  static double calculate_value_for_real_coding(const std::vector<double>& values);

  std::vector<double> history;
};


#endif //TEST1EVALUATIONFUNCTION_H
