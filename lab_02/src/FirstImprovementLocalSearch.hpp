#pragma once
#include <vector>
#include <random>
#include <cmath>
#include <algorithm>
#include <numeric>

class FirstImprovementLocalSearch {
public:
    FirstImprovementLocalSearch(int dimensions,
                                int bits_per_dim,
                                int max_evaluations,
                                double range_min,
                                double range_max,
                                int m_strength = 1)
        : n(dimensions),
          bits(bits_per_dim),
          max_evals(max_evaluations),
          min_range(range_min),
          max_range(range_max),
          m(m_strength),
          eval_count(0) {
        rng.seed(std::random_device{}());
    }

    std::vector<double> run_once(std::vector<double>& history);

private:
    int n;
    int bits;
    int max_evals;
    double min_range, max_range;
    int m;
    int eval_count;
    std::mt19937 rng;

    double evaluation(const std::vector<int>& bits_vec);
    std::vector<std::vector<int>> neighborhood(const std::vector<int>& bits_vec);
    double decode_segment(const std::vector<int>& bits_vec, int start) const;
};

double FirstImprovementLocalSearch::decode_segment(const std::vector<int>& bits_vec, int start) const {
    unsigned int value = 0;
    for (int i = 0; i < bits; ++i)
        value = (value << 1) | bits_vec[start + i];
    double normalized = static_cast<double>(value) / ((1u << bits) - 1u);
    return min_range + normalized * (max_range - min_range);
}

double FirstImprovementLocalSearch::evaluation(const std::vector<int>& bits_vec) {
    ++eval_count;
    double sum = 0.0;
    for (int d = 0; d < n; ++d) {
        double x = decode_segment(bits_vec, d * bits);
        sum += x * x;
    }
    return sum;
}
/// @param bits_vec
/// @return
std::vector<std::vector<int>> FirstImprovementLocalSearch::neighborhood(const std::vector<int>& bits_vec) {
    std::vector<std::vector<int>> neighbors;
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    std::uniform_int_distribution<size_t> bit_dist(0, 15);

    for (int j = 0; j < 16; ++j) {
        std::vector<int> neighbor = bits_vec;

        for (int seg = 0; seg < n; ++seg) {
            if (dist(rng) < m / 16.0) {
                size_t bit_idx = bit_dist(rng);
                neighbor[seg * bits + bit_idx] = 1 - neighbor[seg * bits + bit_idx];
            }
        }

        neighbors.push_back(std::move(neighbor));
    }

    std::shuffle(neighbors.begin(), neighbors.end(), rng);
    return neighbors;
}


std::vector<double> FirstImprovementLocalSearch::run_once(std::vector<double>& history) {
    std::vector<int> current(n * bits, 1);
    eval_count = 0;
    history.clear();
    history.reserve(max_evals);

    double best_val = evaluation(current);
    history.push_back(best_val);

    while (eval_count < max_evals) {
        bool improved = false;
        auto neighbors = neighborhood(current);

        for (const auto& neighbor : neighbors) {
            double val = evaluation(neighbor);
            if (val < best_val) {
                best_val = val;
                current = neighbor;
                improved = true;
                break;
            }
            if (eval_count >= max_evals) break;
        }

        if (!history.empty())
            history.push_back(std::min(history.back(), best_val));
        else
            history.push_back(best_val);

        if (!improved) break;
    }

    while (history.size() < static_cast<size_t>(max_evals))
        history.push_back(history.back());

    return history;
}
