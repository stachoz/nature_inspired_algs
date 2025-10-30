//
// Created by dcend on 30.10.2025.
//

#ifndef DEFAULTBINARYNEIGHBORHOOD_H
#define DEFAULTBINARYNEIGHBORHOOD_H
#include <cstdint>

#include "Neighborhood.h"


class DefaultBinaryNeighborhood : public Neighborhood {
public:
  DefaultBinaryNeighborhood(int number_of_bits_to_manipulate, uint16_t min_dimension_value,
                            uint16_t max_dimension_value);

  Solution *get_neighbor(Solution *solution) override;

private:
  int number_of_bits_to_manipulate;
  uint16_t min_dimension_value;
  uint16_t max_dimension_value;
};


#endif //DEFAULTBINARYNEIGHBORHOOD_H
