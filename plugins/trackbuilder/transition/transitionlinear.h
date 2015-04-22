#ifndef TRANSITIONLINEAR_H
#define TRANSITIONLINEAR_H

#include "transition.h"

class TransitionLinear : public Transition
{
public:
    TransitionLinear(TransitionManager *_transition);
    double getY(double x);
    std::vector<QString> getModes();
};

#endif // TRANSITIONLINEAR_H
