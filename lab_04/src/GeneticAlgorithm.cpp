#include "GeneticAlgorithm.h"

GeneticAlgorithm::GeneticAlgorithm(std::shared_ptr<Evaluation> eval, std::shared_ptr<Solution> start_solution,
    int population_size, int max_evaluations) : evaluation(eval), population_size(population_size), max_evaluations(max_evaluations) {
    dimension = start_solution->get_real_representation().size();
    init_population(start_solution);
}
std::shared_ptr<Solution> GeneticAlgorithm::find_solution() {
    evaluate_population(population);

    while (evaluation->get_eval_counter() < max_evaluations) {
        std::vector<std::shared_ptr<Solution>> offspring;

        while (offspring.size() < population_size) {
            auto parent1 = tournament_select();
            auto parent2 = tournament_select();
            auto children = crossover(parent1, parent2);
            offspring.push_back(children.first);
            if (offspring.size() < population_size) {
                offspring.push_back(children.second);
            }
        }

        for (auto &child: offspring) {
            mutate(child);
        }

        evaluate_population(offspring);

        auto best_it = std::min_element(population.begin(), population.end(), [this](const auto &a, const auto &b) {
            return fitness[a.get()] < fitness[b.get()];
        });
        auto best_solution = *best_it;

        evaluation->add_to_history(fitness[best_solution.get()]);

        population = offspring;
        population[0] = best_solution;
    }

    auto best_it = std::min_element(population.begin(), population.end(), [this](const auto &a, const auto &b) {
        return fitness[a.get()] < fitness[b.get()];
    });
    return *best_it;
}

void GeneticAlgorithm::init_population(std::shared_ptr<Solution> start_solution) {
    population.clear();
    fitness.clear();
    for (int i = 0; i < population_size; i++) {
        auto sol = std::shared_ptr<Solution>(start_solution->clone());
        sol->fit_to_dim(dimension);
        randomize_solution(sol);
        population.push_back(sol);
    }
}

void GeneticAlgorithm::randomize_solution(std::shared_ptr<Solution> sol) {
    if (auto real_sol = std::dynamic_pointer_cast<RealSolution>(sol)) {
        auto &vec = real_sol->get_solution_vector_ref();
        auto [min, max] = real_sol->get_domain();
        std::uniform_real_distribution<double> dist(min, max);
        for (auto &x: vec)
            x = dist(rng);
    } else if (auto bin_sol = std::dynamic_pointer_cast<BinarySolution>(sol)) {
        auto [min, max] = bin_sol->get_domain();
        std::uniform_int_distribution<uint16_t> dist(0, std::numeric_limits<uint16_t>::max());
        for (auto &x: bin_sol->get_solution_vector_ref())
            x = dist(rng);
    }
}

void GeneticAlgorithm::evaluate_population(std::vector<std::shared_ptr<Solution>> &pop) {
    for (auto &sol: pop) {
        if (evaluation->get_eval_counter() >= max_evaluations) {
            break;
        }
        double val = evaluation->evaluate(sol.get());
        fitness[sol.get()] = val;
    }
}

std::shared_ptr<Solution> GeneticAlgorithm::tournament_select() {
    std::uniform_int_distribution<int> dist(0, population_size - 1);
    std::shared_ptr<Solution> best = nullptr;
    double best_val = -1e100;

    for (int i = 0; i < tournament_size; i++) {
        auto candidate = population[dist(rng)];
        double val = fitness[candidate.get()];
        if (!best || val < best_val) {
            best = candidate;
            best_val = val;
        }
    }
    return best;
}

std::pair<std::shared_ptr<Solution>, std::shared_ptr<Solution>>
GeneticAlgorithm::crossover(std::shared_ptr<Solution> parent1, std::shared_ptr<Solution> parent2) {

    std::uniform_real_distribution<double> dist(0.0, 1.0);
    if (dist(rng) > crossover_prob) {
        return {clone_solution(parent1), clone_solution(parent2)};
    }

    auto child1 = clone_solution(parent1);
    auto child2 = clone_solution(parent2);

    if (auto real1 = std::dynamic_pointer_cast<RealSolution>(child1)) {
        auto real2 = std::dynamic_pointer_cast<RealSolution>(child2);
        std::uniform_int_distribution<int> point_dist(1, dimension - 1);
        int point = point_dist(rng);
        auto &v1 = real1->get_solution_vector_ref();
        auto &v2 = real2->get_solution_vector_ref();
        for (int i = point; i < dimension; i++) {
            std::swap(v1[i], v2[i]);
        }
    } else if (auto bin1 = std::dynamic_pointer_cast<BinarySolution>(child1)) {
        auto bin2 = std::dynamic_pointer_cast<BinarySolution>(child2);
        std::uniform_int_distribution<int> point_dist(0, dimension - 1);
        int point = point_dist(rng);
        auto &v1 = bin1->get_solution_vector_ref();
        auto &v2 = bin2->get_solution_vector_ref();
        for (int i = point; i < dimension; i++) {
            std::swap(v1[i], v2[i]);
        }
    }

    return {child1, child2};
}

void GeneticAlgorithm::mutate(std::shared_ptr<Solution> sol) {
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    if (auto real_sol = std::dynamic_pointer_cast<RealSolution>(sol)) {
        auto &vec = real_sol->get_solution_vector_ref();
        auto [min, max] = real_sol->get_domain();
        std::normal_distribution<double> mutation_dist(0.0, 0.1 * (max - min));
        for (auto &x: vec) {
            if (dist(rng) < mutation_prob) {
                x += mutation_dist(rng);
                x = std::min(std::max(x, min), max);
            }
        }
    } else if (auto bin_sol = std::dynamic_pointer_cast<BinarySolution>(sol)) {
        auto &vec = bin_sol->get_solution_vector_ref();
        for (auto &x: vec) {
            if (dist(rng) < mutation_prob)
                x ^= (1 << (rng() % 16));
        }
    }
}

std::shared_ptr<Solution> GeneticAlgorithm::clone_solution(std::shared_ptr<Solution> sol) {
    return std::shared_ptr<Solution>(sol->clone());
}
