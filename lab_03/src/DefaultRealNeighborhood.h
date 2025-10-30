//
// Created by dcend on 30.10.2025.
//

#ifndef DEFAULTREALNEIGHBORHOOD_H
#define DEFAULTREALNEIGHBORHOOD_H
#include "Neighborhood.h"


class DefaultRealNeighborhood : public Neighborhood {
public:
  DefaultRealNeighborhood(double min_dimension_value, double max_dimension_value);

  Solution *get_neighbor(Solution *solution) override;

private:
  double min_dimension_value;
  double max_dimension_value;
};


#endif //DEFAULTREALNEIGHBORHOOD_H
