//
// Created by dcend on 29.10.2025.
//

#ifndef BINARYSOLUTION_H
#define BINARYSOLUTION_H

#include <cstdint>

#include "Solution.h"

class BinarySolution : Solution {
public:
  BinarySolution(size_t dimension, EncodingType encoding_type);

  std::vector<double> get_solution_vector() override;

  void set_solution_vector(std::vector<double> solution_vector) override;

  BinarySolution &operator=(const BinarySolution &other);

protected:
  std::vector<uint16_t> solution_vector;
};


#endif //BINARYSOLUTION_H
