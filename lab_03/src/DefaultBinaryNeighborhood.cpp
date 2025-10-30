//
// Created by dcend on 30.10.2025.
//

#include "DefaultBinaryNeighborhood.h"

#include <bits/random.h>

#include "BinarySolution.h"

DefaultBinaryNeighborhood::DefaultBinaryNeighborhood(int number_of_bits_to_manipulate, uint16_t min_dimension_value,
                                                     uint16_t max_dimension_value) {
  this->number_of_bits_to_manipulate = number_of_bits_to_manipulate;
  this->min_dimension_value = min_dimension_value;
  this->max_dimension_value = max_dimension_value;
}

Solution *DefaultBinaryNeighborhood::get_neighbor(Solution *solution) {
  // BinarySolution *new_neighbor = new BinarySolution(solution->get_dimension(), solution->get_encoding_type());
  //
  // //todo zaimplementowac operator kopiujacy i nastepnie na tej kopi robic operacje przesuwajce bit
  //
  // std::random_device rd;
  // std::mt19937 gen(rd());
  // std::uniform_int_distribution<size_t> dimension_distribution(0, solution->get_dimension() - 1);
  // std::uniform_int_distribution<int> bit_distribution(0, 15);
  //
  // for (int i = 0; i < number_of_bits_to_manipulate; i++) {
  //   // 3
  //   size_t dimension = dimension_distribution(gen);
  //   int bit = bit_distribution(gen);
  //   // new_neighbor.
  // }
}
