//
// Created by dcend on 16.10.2025.
//

#ifndef RANDOMGENERATOR_H
#define RANDOMGENERATOR_H

#include <random>
#include <vector>
#include <thread>
#include <future>
#include <functional>
#include <mutex>
#include <atomic>

namespace rnd {
  template<typename Distribution, typename T = double>
  class RandomGenerator {
  public:
    explicit RandomGenerator(Distribution distribution, unsigned int seed = std::random_device{}());

    T generate();

    std::vector<T> generate_n(std::size_t n);

    std::vector<T> generate_n_parallel(std::size_t n, unsigned int threads = std::thread::hardware_concurrency());

    void generate_stream(size_t total_samples, size_t block_size, std::function<void(const std::vector<T> &)> callback);

    void generate_stream_parallel(size_t total_samples, size_t block_size,
                                  std::function<void(const std::vector<T> &)> callback,
                                  unsigned int threads = std::thread::hardware_concurrency());

  private:
    Distribution distribution;
    std::mt19937 generator;
    mutable std::mutex mutex;
  };
}

#include "RandomGenerator.tpp"

#endif //RANDOMGENERATOR_H
