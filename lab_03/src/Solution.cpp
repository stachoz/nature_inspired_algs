//
// Created by dcend on 29.10.2025.
//

#include "Solution.h"

#include "BinarySolution.h"

size_t Solution::get_dimension() const { return dimension; }

EncodingType Solution::get_encoding_type() const { return encoding_type; }

Solution::Solution(size_t dimension, EncodingType encoding_type) {
    this->dimension = dimension;
    this->encoding_type = encoding_type;
}