//
// Created by dcend on 24.11.2025.
//

#ifndef SALOMON_H
#define SALOMON_H
#include "Evaluation.h"


class Salomon : public Evaluation {
    public:
    double evaluate(Solution *solution) override;
};


#endif // SALOMON_H
