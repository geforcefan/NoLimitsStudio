#ifndef TRANSITIONQUINTIC_H
#define TRANSITIONQUINTIC_H

#include "transition.h"

class TransitionQuintic : public Transition
{
public:
    TransitionQuintic(TransitionManager *_transition);
    double getY(double x);
    std::vector<QString> getModes();

    double getSimpleY(double x);
    double getDoubleBump1Y(double x);
    double getDoubleBump2Y(double x);
};

#endif // TRANSITIONQUINTIC_H
