#pragma once

#include "Evaluation.h"
#include <cmath>
#include <cstdint>

class Test1EvaluationFunction : public Evaluation {
public:
    double evaluate(Solution *solution) override;

    std::vector<double> &get_history() override;

    void clear_history() override;

    static uint16_t gray_to_binary(uint16_t gray);

private:
    static double calculate_value_for_binary_coding(const std::vector<double> &values);

    static double calculate_value_for_grey_coding(const std::vector<double> &values);

    static double calculate_value_for_real_coding(const std::vector<double> &values);

    std::vector<double> history;
};