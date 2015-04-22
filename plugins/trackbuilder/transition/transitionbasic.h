#ifndef TRANSITIONBASIC_H
#define TRANSITIONBASIC_H

#include "transition.h"

class TransitionBasic : public Transition
{
public:
    TransitionBasic(TransitionManager *_transition);
    double getY(double x);
    std::vector<QString> getModes();
};

#endif // TRANSITIONBASIC_H
