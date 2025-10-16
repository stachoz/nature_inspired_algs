#include "CircleAreaExp.hpp"
#include <filesystem>

int main() {
    std::filesystem::path file (std::string(RESULTS_DIR) + "/points.csv");
    CircleAreaExp exp(1'000'000, file);
    exp.generate_data();
    exp.calculate_results({
        1000,
        10'000,
        100'000,
    });
}
