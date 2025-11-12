#pragma once
#include <bits/stl_algo.h>
#include <limits>
#include "Solution.h"

class RealSolution : public Solution {
public:
    explicit RealSolution(const std::pair<double, double>& func_domain) {
        domain = func_domain;
    }

    std::vector<double> get_real_representation() override;
    Solution* clone() const override;


    void fit_to_dim(int dim) override {
        solution_vector = std::vector<double>(dim, domain.second);
    }

    std::vector<double>& get_solution_vector_ref() {
        return solution_vector;
    }
    std::pair<double,double> get_domain() const { return domain; }


private:
    std::vector<double> solution_vector;
    std::pair<double, double> domain;
};
