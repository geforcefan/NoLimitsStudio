#include "transitionlinear.h"
#include "transitionmanager.h"
#include <math.h>

TransitionLinear::TransitionLinear(TransitionManager *_transition) : Transition(_transition) {

}

double TransitionLinear::getY(double x) {
    return lastY = ((x/transition->getChangeX()) * transition->getChangeY()) + transition->getStartY();
}


std::vector<QString> TransitionLinear::getModes() {
    std::vector<QString> retVals;
    return retVals;
}
