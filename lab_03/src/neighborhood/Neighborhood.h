#pragma once
#include <memory>


#include "solutions/Solution.h"

class Neighborhood {
public:
    virtual std::shared_ptr<Solution> get_neighbor(std::shared_ptr<Solution> solution) = 0;

    virtual ~Neighborhood() = default;
};
