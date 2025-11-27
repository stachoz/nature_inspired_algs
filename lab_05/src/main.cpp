
#include <filesystem>
#include <iostream>
#include "CSVFile.h"
#include "evaluation/Evaluation.h"
#include "evaluation/GeneralizedRosenbrock.h"
#include "evaluation/Salomon.h"
#include "evaluation/Whitley.h"
#include "solutions/RealSolution.h"

struct Experiment {
    std::string name;
    Evaluation *func;
    std::pair<double, double> domain;
};

int main() {
    const int n_points = 100;
    const int dim = 2;

    GeneralizedRosenbrock rosenbrock;
    Salomon salomon;
    Whitley whitley;

    std::vector<Experiment> experiments = {{"GeneralizedRosenbrock", &rosenbrock, {-30.0, 30.0}},
                                           {"Salomon", &salomon, {-100.0, 100.0}},
                                           {"Whitley", &whitley, {-10.24, 10.24}}};

    for (auto &exp: experiments) {
        std::filesystem::path output = std::filesystem::path(RESULTS_DIR) / (exp.name + ".csv");
        CSVFile csv(output);
        csv.append_row("x", "y", "f");

        for (int i = 0; i < n_points; i++) {
            double x_val = exp.domain.first + i * (exp.domain.second - exp.domain.first) / (n_points - 1);
            for (int j = 0; j < n_points; j++) {
                double y_val = exp.domain.first + j * (exp.domain.second - exp.domain.first) / (n_points - 1);

                RealSolution solution(exp.domain);
                solution.fit_to_dim(dim);
                auto &vec = solution.get_solution_vector_ref();
                vec[0] = x_val;
                vec[1] = y_val;

                double f_val = exp.func->evaluate(&solution);
                csv.append_row(x_val, y_val, f_val);
            }
        }

        std::cout << "Zapisano wyniki do '" << csv.get_filepath() << "'\n";
    }

    return 0;
}