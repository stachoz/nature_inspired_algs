//
// Created by dcend on 16.10.2025.
//

#include <algorithm>
#include "RandomGenerator.hpp"

namespace rnd {
  template<typename Distribution, typename T>
  RandomGenerator<Distribution, T>::RandomGenerator(Distribution distribution, unsigned int seed)
    : distribution(distribution), generator(seed) {
  }

  template<typename Distribution, typename T>
  T RandomGenerator<Distribution, T>::generate() {
    std::lock_guard<std::mutex> lock(mutex);
    return distribution(generator);
  }

  template<typename Distribution, typename T>
  std::vector<T> RandomGenerator<Distribution, T>::generate_n(size_t n) {
    std::vector<T> data;
    data.reserve(n);
    for (size_t i = 0; i < n; ++i)
      data.push_back(distribution(generator));
    return data;
  }

  template<typename Distribution, typename T>
  std::vector<T> RandomGenerator<Distribution, T>::generate_n_parallel(size_t n, unsigned int threads) {
    if (threads == 0) threads = 1;
    size_t chunk = n / threads;

    std::vector<std::future<std::vector<T> > > futures;
    futures.reserve(threads);

    for (unsigned int t = 0; t < threads; ++t) {
      futures.push_back(std::async(std::launch::async, [&, t]() {
        std::mt19937 local_gen(generator() + t * 1337);
        Distribution local_dist = distribution;
        std::vector<T> part;
        part.reserve(chunk);
        for (size_t j = 0; j < chunk; ++j)
          part.push_back(local_dist(local_gen));
        return part;
      }));
    }

    std::vector<T> data;
    data.reserve(n);

    for (auto &f: futures) {
      auto part = f.get();
      data.insert(data.end(), part.begin(), part.end());
    }

    for (size_t i = chunk * threads; i < n; ++i)
      data.push_back(distribution(generator));

    return data;
  }

  template<typename Distribution, typename T>
  void RandomGenerator<Distribution, T>::generate_stream(
    size_t total_samples, size_t block_size,
    std::function<void(const std::vector<T> &)> callback) {
    std::vector<T> buffer;
    buffer.reserve(block_size);

    size_t generated = 0;
    while (generated < total_samples) {
      size_t to_generate = std::min(block_size, total_samples - generated);
      buffer.clear();
      for (size_t i = 0; i < to_generate; ++i)
        buffer.push_back(distribution(generator));
      callback(buffer);
      generated += to_generate;
    }
  }

  template<typename Distribution, typename T>
  void RandomGenerator<Distribution, T>::generate_stream_parallel(
    size_t total_samples, size_t block_size,
    std::function<void(const std::vector<T> &)> callback,
    unsigned int threads) {
    if (threads == 0) threads = std::thread::hardware_concurrency();
    size_t total_blocks = (total_samples + block_size - 1) / block_size;

    std::atomic<size_t> next_block{0};
    std::vector<std::thread> workers;

    for (unsigned int t = 0; t < threads; ++t) {
      workers.emplace_back([&, t]() {
        std::mt19937 local_gen(generator() + t * 1337);
        Distribution local_dist = distribution;
        std::vector<T> buffer;
        buffer.reserve(block_size);

        while (true) {
          size_t block_idx = next_block.fetch_add(1);
          if (block_idx >= total_blocks)
            break;

          size_t start = block_idx * block_size;
          size_t remaining = total_samples - start;
          size_t current_block_size = std::min(block_size, remaining);

          buffer.clear();
          for (size_t i = 0; i < current_block_size; ++i)
            buffer.push_back(local_dist(local_gen));

          callback(buffer);
        }
      });
    }

    for (auto &w: workers)
      w.join();
  }
}
