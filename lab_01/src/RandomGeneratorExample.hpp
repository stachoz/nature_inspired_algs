//
// Created by dcend on 16.10.2025.
//

#ifndef RANDOMGENERATOREXAMPLE_H
#define RANDOMGENERATOREXAMPLE_H

#include "random/RandomGenerator.hpp"
#include <random>
#include "CSVFile.h"

class RandomGeneratorExample {
public:
  void run_uniform_distribution() {
    generate_and_save(std::uniform_real_distribution<double>(0.0, 1.0), "uniform_real_distribution.csv");
  }

  void run_noraml_distribution() {
    generate_and_save(std::normal_distribution<double>(0.0, 5.0), "normal_distribution.csv");
  }

private:
  template<typename Distribution>
  void generate_and_save(Distribution distribution, std::string output_csv_filename) {
    std::filesystem::path file(std::string(RESULTS_DIR) + '/' + output_csv_filename);
    CSVFile csv_file(file);

    rnd::RandomGenerator generator(distribution);
    generator.generate_stream_parallel(total_samples, block_size, [&csv_file](const std::vector<double> &block) {
      for (int i = 0; i < block.size(); i++) {
        csv_file.append_row(block[i]);
      }
    });
  }


  const int total_samples = 10000;
  const int block_size = 100;
};

#endif //RANDOMGENERATOREXAMPLE_H
