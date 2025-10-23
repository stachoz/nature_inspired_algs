#include <filesystem>
#include <fstream>
#include <iostream>
#include "FirstImprovementLocalSearch.hpp"

void save_csv(const std::string& filename, const std::vector<double>& avg_series) {
    std::ofstream out(filename);
    for (size_t i = 0; i < avg_series.size(); ++i)
        out << i + 1 << "," << avg_series[i] << "\n";
    std::cout << "Zapisano: " << filename << " (" << avg_series.size() << " rekordów)\n";
}

int main() {
    const int bits = 16;
    const int evals = 10000;
    const int runs = 100;
    const double minR = -10.0, maxR = 10.0;
    const int m = 8;

    std::vector<int> dimensions = {2, 5, 10};

    for (int n: dimensions) {
        std::cout << "Uruchamiam eksperyment dla n = " << n << "...\n";
        std::vector<double> avg_series(evals, 0.0);

        for (int run = 0; run < runs; ++run) {
            FirstImprovementLocalSearch ls(n, bits, evals, minR, maxR, m);
            std::vector<double> history;
            ls.run_once(history);
            for (int i = 0; i < evals; ++i)
                avg_series[i] += history[i];
        }

        for (double &v: avg_series) {
            v /= runs;
        }

        save_csv(std::string(RESULTS_DIR) + "/results_n" + std::to_string(n) + ".csv", avg_series);
    }

    std::cout << "\n Eksperymenty zakończone.\n";

    std::filesystem::path cwd = std::filesystem::current_path();

    return 0;
}
