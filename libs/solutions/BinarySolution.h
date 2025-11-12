#pragma once

#include <complex>
#include <cstdint>
#include <ranges>

#include "Solution.h"

class BinarySolution : public Solution {
public:
    explicit BinarySolution(const std::pair<double, double>& func_domain) {
        domain = func_domain;
    }

    std::vector<double> get_real_representation() override;
    Solution* clone() const override;


    void fit_to_dim(int dim) override {
        solution_vector = std::vector<uint16_t>(dim, std::numeric_limits<uint16_t>::max());
    }

    std::vector<uint16_t>& get_solution_vector_ref() {
        return solution_vector;
    }

    std::pair<double,double> get_domain() const { return domain; }


private:
    std::vector<uint16_t> solution_vector;
    std::pair<double, double> domain;
};