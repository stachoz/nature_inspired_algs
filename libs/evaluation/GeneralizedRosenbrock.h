//
// Created by dcend on 24.11.2025.
//

#ifndef GENERALIZEDROSENBROCK_H
#define GENERALIZEDROSENBROCK_H
#include "Evaluation.h"


class GeneralizedRosenbrock : public Evaluation {
public:
    double evaluate(Solution *solution) override;
};


#endif // GENERALIZEDROSENBROCK_H
