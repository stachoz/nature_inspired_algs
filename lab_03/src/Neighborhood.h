//
// Created by dcend on 29.10.2025.
//

#ifndef NEIGHBORHOOD_H
#define NEIGHBORHOOD_H
#include "Solution.h"

class Neighborhood {
public:
  virtual Solution *get_neighbor(Solution *solution) = 0;

  virtual ~Neighborhood() = default;
};

#endif //NEIGHBORHOOD_H
