#include "transitionbump.h"
#include "transitionmanager.h"
#include <math.h>

TransitionBump::TransitionBump(TransitionManager *_transition) : Transition(_transition) {

}

double TransitionBump::getY(double x) {
    x = 1 * x / transition->getChangeX();
    x = transition->sinFuncB(x, transition->getCenter());
    x = transition->sinFuncA(x, transition->getTension());
    double func = transition->getStartY() + ((0.5 + (0.5 * sin((x * (M_PI*2) ) - (M_PI/2)))) * transition->getChangeY());

    return lastY = func;
}


std::vector<QString> TransitionBump::getModes() {
    std::vector<QString> retVals;
    return retVals;
}
