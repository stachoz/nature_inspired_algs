#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <filesystem>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

constexpr int POP_SIZE = 100;
constexpr int MAX_GEN = 500;
constexpr double CROSS_PROB = 0.9;
constexpr double ETA_C_DEF = 20.0;
constexpr double ETA_M_DEF = 20.0;

struct Individual {
    std::vector<double> genes;
    std::vector<double> objectives;
    int rank = 0;
    double crowding_dist = 0.0;
    Individual(int n_vars) : genes(n_vars), objectives(2) {}
};

void evaluate(Individual& ind, const std::string& name, int n) {
    double f1 = 0, g = 0, h = 0;
    if (name == "ZDT1") {
        f1 = ind.genes[0];
        for (int i = 1; i < n; ++i) g += ind.genes[i];
        g = 1.0 + 9.0 * g / (n - 1);
        h = 1.0 - std::sqrt(f1 / g);
    } else if (name == "ZDT2") {
        f1 = ind.genes[0];
        for (int i = 1; i < n; ++i) g += ind.genes[i];
        g = 1.0 + 9.0 * g / (n - 1);
        h = 1.0 - std::pow(f1 / g, 2.0);
    } else if (name == "ZDT3") {
        f1 = ind.genes[0];
        for (int i = 1; i < n; ++i) g += ind.genes[i];
        g = 1.0 + 9.0 * g / (n - 1);
        h = 1.0 - std::sqrt(f1 / g) - (f1 / g) * std::sin(10.0 * M_PI * f1);
    } else if (name == "ZDT4") {
        f1 = ind.genes[0];
        for (int i = 1; i < n; ++i)
            g += (std::pow(ind.genes[i], 2.0) - 10.0 * std::cos(4.0 * M_PI * ind.genes[i]));
        g = 1.0 + 10.0 * (n - 1) + g;
        h = 1.0 - std::sqrt(f1 / g);
    } else if (name == "ZDT6") {
        f1 = 1.0 - std::exp(-4.0 * ind.genes[0]) * std::pow(std::sin(6.0 * M_PI * ind.genes[0]), 6.0);
        for (int i = 1; i < n; ++i) g += ind.genes[i];
        g = 1.0 + 9.0 * std::pow(g / (n - 1), 0.25);
        h = 1.0 - std::pow(f1 / g, 2.0);
    }
    ind.objectives[0] = f1;
    ind.objectives[1] = g * h;
}

std::mt19937 gen_rand(std::random_device{}());

bool dominates(const Individual& a, const Individual& b) {
    bool better = false;
    for (int i = 0; i < 2; ++i) {
        if (a.objectives[i] > b.objectives[i]) return false;
        if (a.objectives[i] < b.objectives[i]) better = true;
    }
    return better;
}

void compute_ranks_and_distance(std::vector<Individual>& pop) {
    int n = (int)pop.size();
    std::vector<int> sp_count(n, 0);
    std::vector<std::vector<int>> sets(n);
    std::vector<int> front;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (dominates(pop[i], pop[j])) sets[i].push_back(j);
            else if (dominates(pop[j], pop[i])) sp_count[i]++;
        }
        if (sp_count[i] == 0) { pop[i].rank = 0; front.push_back(i); }
    }
    int r = 0;
    while (!front.empty()) {
        std::vector<int> next_front;
        for (int i : front) {
            for (int j : sets[i]) {
                sp_count[j]--;
                if (sp_count[j] == 0) { pop[j].rank = r + 1; next_front.push_back(j); }
            }
        }
        r++; front = next_front;
    }
    for (auto& ind : pop) ind.crowding_dist = 0;
    for (int m = 0; m < 2; ++m) {
        std::sort(pop.begin(), pop.end(), [m](const Individual& a, const Individual& b) { return a.objectives[m] < b.objectives[m]; });
        pop[0].crowding_dist = pop[n - 1].crowding_dist = 1e10;
        double range = pop[n - 1].objectives[m] - pop[0].objectives[m];
        if (range > 1e-9) {
            for (int i = 1; i < n - 1; ++i) pop[i].crowding_dist += (pop[i + 1].objectives[m] - pop[i - 1].objectives[m]) / range;
        }
    }
}

void mutate(Individual& ind, const std::string& name, int n, double eta_m) {
    double mut_prob = 1.0 / n;
    std::uniform_real_distribution<> dis(0, 1);
    for (int i = 0; i < n; ++i) {
        if (dis(gen_rand) < mut_prob) {
            double y = ind.genes[i], yl = (name == "ZDT4" && i > 0) ? -5.0 : 0.0, yu = (name == "ZDT4" && i > 0) ? 5.0 : 1.0;
            double rnd = dis(gen_rand);
            double deltaq = (rnd < 0.5) ? std::pow(2.0 * rnd, 1.0 / (eta_m + 1.0)) - 1.0 : 1.0 - std::pow(2.0 * (1.0 - rnd), 1.0 / (eta_m + 1.0));
            ind.genes[i] = std::clamp(y + deltaq * (yu - yl), yl, yu);
        }
    }
}

std::pair<Individual, Individual> crossover(const Individual& p1, const Individual& p2, const std::string& name, int n, double eta_c) {
    Individual c1 = p1, c2 = p2;
    std::uniform_real_distribution<> dis(0, 1);
    if (dis(gen_rand) < CROSS_PROB) {
        for (int i = 0; i < n; ++i) {
            if (dis(gen_rand) < 0.5 && std::abs(p1.genes[i] - p2.genes[i]) > 1e-14) {
                double y1 = std::min(p1.genes[i], p2.genes[i]), y2 = std::max(p1.genes[i], p2.genes[i]);
                double yl = (name == "ZDT4" && i > 0) ? -5.0 : 0.0, yu = (name == "ZDT4" && i > 0) ? 5.0 : 1.0;
                double rnd = dis(gen_rand), beta = 1.0 + (2.0 * (y1 - yl) / (y2 - y1)), alpha = 2.0 - std::pow(beta, -(eta_c + 1.0));
                double betaq = (rnd <= (1.0 / alpha)) ? std::pow(rnd * alpha, 1.0 / (eta_c + 1.0)) : std::pow(1.0 / (2.0 - rnd * alpha), 1.0 / (eta_c + 1.0));
                c1.genes[i] = std::clamp(0.5 * ((y1 + y2) - betaq * (y2 - y1)), yl, yu);
                c2.genes[i] = std::clamp(0.5 * ((y1 + y2) + betaq * (y2 - y1)), yl, yu);
            }
        }
    }
    return {c1, c2};
}

void run_experiment(std::string name, int dim) {
    std::filesystem::path file_path = std::filesystem::path(RESULTS_DIR) / (name + "_dim" + std::to_string(dim) + ".csv");
    std::ofstream out(file_path);
    out << "iteration,f1,f2\n";
    double ec = (name == "ZDT4") ? 10.0 : ETA_C_DEF, em = (name == "ZDT4") ? 10.0 : ETA_M_DEF;
    std::vector<Individual> P;
    for (int i = 0; i < POP_SIZE; ++i) {
        Individual ind(dim);
        for (int j = 0; j < dim; ++j) {
            double yl = (name == "ZDT4" && j > 0) ? -5.0 : 0.0, yu = (name == "ZDT4" && j > 0) ? 5.0 : 1.0;
            ind.genes[j] = std::uniform_real_distribution<>(yl, yu)(gen_rand);
        }
        evaluate(ind, name, dim); P.push_back(ind);
    }
    for (int gen = 1; gen <= MAX_GEN; ++gen) {
        compute_ranks_and_distance(P);
        if (gen == 20 || gen == 50 || gen == 100 || gen == 500) {
            for (const auto& ind : P) if (ind.rank == 0) out << gen << "," << ind.objectives[0] << "," << ind.objectives[1] << "\n";
        }
        std::vector<Individual> Q;
        while (Q.size() < POP_SIZE) {
            auto tournament = [&]() {
                int i = std::uniform_int_distribution<>(0, (int)P.size() - 1)(gen_rand), j = std::uniform_int_distribution<>(0, (int)P.size() - 1)(gen_rand);
                if (P[i].rank < P[j].rank) return i; if (P[j].rank < P[i].rank) return j;
                return (P[i].crowding_dist > P[j].crowding_dist) ? i : j;
            };
            auto [c1, c2] = crossover(P[tournament()], P[tournament()], name, dim, ec);
            mutate(c1, name, dim, em); mutate(c2, name, dim, em);
            evaluate(c1, name, dim); evaluate(c2, name, dim);
            Q.push_back(c1); Q.push_back(c2);
        }
        P.insert(P.end(), Q.begin(), Q.end());
        compute_ranks_and_distance(P);
        std::sort(P.begin(), P.end(), [](const Individual& a, const Individual& b) {
            if (a.rank != b.rank) return a.rank < b.rank; return a.crowding_dist > b.crowding_dist;
        });
        P.erase(P.begin() + POP_SIZE, P.end());
    }
}

int main() {
    std::vector<std::string> problems = {"ZDT1", "ZDT2", "ZDT3", "ZDT4", "ZDT6"};
    std::vector<int> dims = {10, 30, 50};
    for (const auto& p : problems) for (int d : dims) run_experiment(p, d);
    return 0;
}