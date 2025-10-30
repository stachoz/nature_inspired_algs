//
// Created by dcend on 28.10.2025.
//

#ifndef EVALUATION_H
#define EVALUATION_H
#include "Solution.h"


class Evaluation {
public:
  virtual double evaluate(Solution* solution) = 0;

  virtual ~Evaluation() = default;
};

#endif //EVALUATION_H
