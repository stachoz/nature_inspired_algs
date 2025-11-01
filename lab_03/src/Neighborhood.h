#pragma once

#include "Solution.h"

class Neighborhood {
public:
    virtual Solution *get_neighbor(Solution *solution) = 0;

    virtual ~Neighborhood() = default;
};
