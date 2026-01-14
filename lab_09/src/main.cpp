#include <algorithm>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <numeric>
#include <random>
#include <ranges>
#include <vector>

constexpr int POPULATION_SIZE = 100;
constexpr int MAX_EVALUATIONS = 20000;
constexpr int NUM_VARIABLES = 30;
constexpr int NUM_OBJECTIVES = 2;

constexpr double CROSSOVER_PROB = 0.9;
constexpr double MUTATION_PROB = 1.0 / NUM_VARIABLES;
constexpr double ETA_C = 20.0; // Indeks dystrybucji krzyżowania
constexpr double ETA_M = 20.0; // Indeks dystrybucji mutacji

constexpr double VAR_MIN = 0.0;
constexpr double VAR_MAX = 1.0;

struct Individual {
    std::vector<double> genes;
    std::vector<double> objectives;
    int rank = 0;
    double crowding_distance = 0.0;

    Individual() : genes(NUM_VARIABLES), objectives(NUM_OBJECTIVES) {}
};

std::random_device rd;
std::mt19937 gen(rd());

double random_double(double min, double max) {
    std::uniform_real_distribution<> dis(min, max);
    return dis(gen);
}

// f1(x) = x1
// g(x) = 1 + 9 * sum(xi) / (m-1)
// h(f1, g) = 1 - sqrt(f1/g)
// f2(x) = g * h
void evaluate_zdt1(Individual& ind) {
    double f1 = ind.genes[0];

    double sum = 0.0;
    for (int i = 1; i < NUM_VARIABLES; ++i) {
        sum += ind.genes[i];
    }
    double g = 1.0 + 9.0 * sum / (NUM_VARIABLES - 1);
    double h = 1.0 - std::sqrt(f1 / g);
    double f2 = g * h;

    ind.objectives[0] = f1;
    ind.objectives[1] = f2;
}


// Mutacja Wielomianowa
void mutate(Individual& ind) {
    std::uniform_real_distribution<> dis(0.0, 1.0);
    for (int i = 0; i < NUM_VARIABLES; ++i) {
        if (dis(gen) < MUTATION_PROB) {
            double y = ind.genes[i];
            double yl = VAR_MIN;
            double yu = VAR_MAX;
            double delta1 = (y - yl) / (yu - yl);
            double delta2 = (yu - y) / (yu - yl);
            double rnd = dis(gen);
            double mut_pow = 1.0 / (ETA_M + 1.0);
            double deltaq;

            if (rnd <= 0.5) {
                double xy = 1.0 - delta1;
                double val = 2.0 * rnd + (1.0 - 2.0 * rnd) * (std::pow(xy, (ETA_M + 1.0)));
                deltaq = std::pow(val, mut_pow) - 1.0;
            } else {
                double xy = 1.0 - delta2;
                double val = 2.0 * (1.0 - rnd) + 2.0 * (rnd - 0.5) * (std::pow(xy, (ETA_M + 1.0)));
                deltaq = 1.0 - std::pow(val, mut_pow);
            }

            y = y + deltaq * (yu - yl);
            ind.genes[i] = std::clamp(y, VAR_MIN, VAR_MAX);
        }
    }
}

// Krzyżowanie SBX (Simulated Binary Crossover)
std::pair<Individual, Individual> crossover(const Individual& p1, const Individual& p2) {
    Individual c1 = p1;
    Individual c2 = p2;
    std::uniform_real_distribution<> dis(0.0, 1.0);

    if (dis(gen) <= CROSSOVER_PROB) {
        for (int i = 0; i < NUM_VARIABLES; ++i) {
            if (dis(gen) <= 0.5) {
                if (std::abs(p1.genes[i] - p2.genes[i]) > 1.0e-14) {
                    double y1 = std::min(p1.genes[i], p2.genes[i]);
                    double y2 = std::max(p1.genes[i], p2.genes[i]);
                    double yl = VAR_MIN;
                    double yu = VAR_MAX;
                    double rand = dis(gen);
                    double beta = 1.0 + (2.0 * (y1 - yl) / (y2 - y1));
                    double alpha = 2.0 - std::pow(beta, -(ETA_C + 1.0));
                    double betaq;

                    if (rand <= (1.0 / alpha)) {
                        betaq = std::pow((rand * alpha), (1.0 / (ETA_C + 1.0)));
                    } else {
                        betaq = std::pow((1.0 / (2.0 - rand * alpha)), (1.0 / (ETA_C + 1.0)));
                    }

                    double c1_val = 0.5 * ((y1 + y2) - betaq * (y2 - y1));
                    double c2_val = 0.5 * ((y1 + y2) + betaq * (y2 - y1));

                    c1.genes[i] = std::clamp(c1_val, VAR_MIN, VAR_MAX);
                    c2.genes[i] = std::clamp(c2_val, VAR_MIN, VAR_MAX);
                }
            }
        }
    }
    return {c1, c2};
}

bool dominates(const Individual& a, const Individual& b) {
    bool at_least_one_better = false;
    for (int i = 0; i < NUM_OBJECTIVES; ++i) {
        if (a.objectives[i] > b.objectives[i]) {
            return false; // A jest gorsze w czymkolwiek -> nie dominuje
        }
        if (a.objectives[i] < b.objectives[i]) {
            at_least_one_better = true;
        }
    }
    return at_least_one_better;
}

std::vector<std::vector<int>> fast_non_dominated_sort(std::vector<Individual>& pop) {
    std::vector<std::vector<int>> fronts;
    std::vector<std::vector<int>> S(pop.size());
    std::vector<int> n(pop.size(), 0);

    std::vector<int> current_front;
    for (size_t p = 0; p < pop.size(); ++p) {
        for (size_t q = 0; q < pop.size(); ++q) {
            if (dominates(pop[p], pop[q])) {
                S[p].push_back(q);
            } else if (dominates(pop[q], pop[p])) {
                n[p]++;
            }
        }
        if (n[p] == 0) {
            pop[p].rank = 0;
            current_front.push_back(p);
        }
    }
    fronts.push_back(current_front);

    int i = 0;
    while (i < fronts.size()) {
        std::vector<int> next_front;

        for (int p_idx : fronts[i]) {
            for (int q_idx : S[p_idx]) {
                n[q_idx]--;
                if (n[q_idx] == 0) {
                    pop[q_idx].rank = i + 1;
                    next_front.push_back(q_idx);
                }
            }
        }

        i++;

        if (!next_front.empty()) {
            fronts.push_back(next_front);
        }
    }

    return fronts;
}

void calculate_crowding_distance(std::vector<Individual>& pop, std::vector<int>& front) {
    if (front.empty()) return;

    for (int idx : front) {
        pop[idx].crowding_distance = 0.0;
    }

    for (int m = 0; m < NUM_OBJECTIVES; ++m) {
        std::sort(front.begin(), front.end(), [&](int a, int b) {
            return pop[a].objectives[m] < pop[b].objectives[m];
        });

        pop[front.front()].crowding_distance = std::numeric_limits<double>::infinity();
        pop[front.back()].crowding_distance = std::numeric_limits<double>::infinity();

        double min_obj = pop[front.front()].objectives[m];
        double max_obj = pop[front.back()].objectives[m];
        double range = max_obj - min_obj;

        if (range == 0) continue;

        for (size_t i = 1; i < front.size() - 1; ++i) {
            if (std::isinf(pop[front[i]].crowding_distance)) continue;

            double dist = (pop[front[i+1]].objectives[m] - pop[front[i-1]].objectives[m]) / range;
            pop[front[i]].crowding_distance += dist;
        }
    }
}

int tournament_selection(const std::vector<Individual>& pop) {
    std::uniform_int_distribution<> dis(0, pop.size() - 1);
    int idx1 = dis(gen);
    int idx2 = dis(gen);

    const auto& ind1 = pop[idx1];
    const auto& ind2 = pop[idx2];

    if (ind1.rank < ind2.rank) return idx1;
    if (ind2.rank < ind1.rank) return idx2;

    if (ind1.crowding_distance > ind2.crowding_distance) return idx1;
    return idx2;
}


int main() {
    std::vector<Individual> P(POPULATION_SIZE);
    for (auto& ind : P) {
        for (auto& gene : ind.genes) {
            gene = random_double(VAR_MIN, VAR_MAX);
        }
        evaluate_zdt1(ind);
    }

    int evaluations = POPULATION_SIZE;
    int generation = 0;

    while (evaluations < MAX_EVALUATIONS) {
        generation++;

        auto fronts = fast_non_dominated_sort(P);
        for (auto& front : fronts) {
            calculate_crowding_distance(P, front);
        }

        std::vector<Individual> Q;
        while (Q.size() < POPULATION_SIZE) {
            int p1_idx = tournament_selection(P);
            int p2_idx = tournament_selection(P);

            auto children = crossover(P[p1_idx], P[p2_idx]);
            mutate(children.first);
            mutate(children.second);

            evaluate_zdt1(children.first);
            evaluate_zdt1(children.second);

            Q.push_back(children.first);
            Q.push_back(children.second);
            evaluations += 2;
        }

        std::vector<Individual> R = P;
        R.insert(R.end(), Q.begin(), Q.end());

        fronts = fast_non_dominated_sort(R);

        std::vector<Individual> P_new;
        for (auto& front : fronts) {
            if (P_new.size() + front.size() <= POPULATION_SIZE) {
                calculate_crowding_distance(R, front);
                for (int idx : front) {
                    P_new.push_back(R[idx]);
                }
            } else {
                calculate_crowding_distance(R, front);
                std::sort(front.begin(), front.end(), [&](int a, int b) {
                    return R[a].crowding_distance > R[b].crowding_distance; // Malejąco
                });

                int remaining = POPULATION_SIZE - P_new.size();
                for (int i = 0; i < remaining; ++i) {
                    P_new.push_back(R[front[i]]);
                }
                break;
            }
        }
        P = P_new;
    }

    auto final_fronts = fast_non_dominated_sort(P);

    std::filesystem::path output_file = std::filesystem::path(RESULTS_DIR) / "zdt1_results.csv";
    std::ofstream outfile(output_file);
    outfile << "f1,f2\n";

    for (int idx : final_fronts[0]) {
        outfile << P[idx].objectives[0] << "," << P[idx].objectives[1] << "\n";
    }

    std::cout << "Wykonano " << evaluations << " ewaluacji.\n";
    std::cout << "Zapisano " << final_fronts[0].size() << " rozwiazań niezdominowanych do 'zdt1_results.csv'.\n";

    return 0;
}