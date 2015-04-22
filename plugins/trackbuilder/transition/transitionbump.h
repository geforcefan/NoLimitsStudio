#ifndef TRANSITIONBUMP_H
#define TRANSITIONBUMP_H

#include "transition.h"

class TransitionBump : public Transition
{
public:
    TransitionBump(TransitionManager *_transition);
    double getY(double x);
    std::vector<QString> getModes();
};

#endif // TRANSITIONBUMP_H
