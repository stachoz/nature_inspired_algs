//
// Created by dcend on 29.10.2025.
//

#ifndef SOLUTION_H
#define SOLUTION_H

#include <cstddef>
#include "vector"
#include "EncodingType.h"

class Solution {
public:
  Solution(size_t dimension, EncodingType encoding_type);

  Solution(const Solution &other);

  virtual ~Solution() = default;

  virtual std::vector<double> get_solution_vector() = 0;

  virtual void set_solution_vector(std::vector<double> solution_vector) = 0;

  Solution &operator=(const Solution &other);

  size_t get_dimension() const;

  EncodingType get_encoding_type() const;

  virtual void print() = 0;

protected:
  size_t dimension;
  EncodingType encoding_type;
};

#endif //SOLUTION_H
