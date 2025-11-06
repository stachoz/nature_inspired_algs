#pragma once

#include "solutions/Solution.h"


class Evaluation {
public:
    virtual ~Evaluation() = default;
    virtual double evaluate(Solution *solution) = 0;

    std::vector<double> &get_history() { return history; }
    void clear_state() {
        eval_counter = 0;
        history.clear();
    }
    int get_eval_counter() const { return eval_counter; }
    void increase_counter() {
        eval_counter++;
    }
    void decrease_counter() {
        eval_counter--;
    }

    void add_to_history(double value) {
        history.push_back(value);
    }

    std::optional<double> get_last_eval_value() const {
        if(history.empty()) {
            return std::nullopt;
        }

        return history.back();
    }

private:
    int eval_counter = 0;
    std::vector<double> history {};
};

