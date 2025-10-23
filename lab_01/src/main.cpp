#include <filesystem>
#include "CircleAreaExp.hpp"
#include "RandomGeneratorExample.hpp"

void circle_example() {
    std::filesystem::path file(std::string(RESULTS_DIR) + "/points.csv");
    CircleAreaExp exp(1'000'000, file);
    exp.generate_data();
    exp.calculate_results({
            1000,
            10'000,
            100'000,
    });
}

void random_generator_example() {
    RandomGeneratorExample generator_example;
    generator_example.run_uniform_distribution();
    generator_example.run_noraml_distribution();
}

int main() {
    random_generator_example();
    circle_example();
}
