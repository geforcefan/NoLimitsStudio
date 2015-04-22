#ifndef TRANSITIONGENERIC_H
#define TRANSITIONGENERIC_H

#include "transition.h"

class TransitionGeneric : public Transition
{
public:
    TransitionGeneric(TransitionManager *_transition);
    double getY(double x);
    std::vector<QString> getModes();

    bool useSlope;
    double slope;
    double shape1;
    double shape2;
};

#endif // TRANSITIONGENERIC_H
