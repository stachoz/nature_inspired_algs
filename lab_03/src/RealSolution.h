//
// Created by dcend on 29.10.2025.
//

#ifndef REALSOLUTION_H
#define REALSOLUTION_H
#include "Solution.h"


class RealSolution : public Solution {
public:
  RealSolution(size_t dimension, EncodingType encoding_type);

  std::vector<double> get_solution_vector() override;

  void set_solution_vector(std::vector<double> solution_vector) override;

  RealSolution &operator=(const RealSolution &other);

  RealSolution(const RealSolution &other);

protected:
  std::vector<double> solution_vector;
};


#endif //REALSOLUTION_H
