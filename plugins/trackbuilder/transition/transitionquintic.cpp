#include "transitionquintic.h"
#include "transitionmanager.h"
#include <math.h>

TransitionQuintic::TransitionQuintic(TransitionManager *_transition) : Transition(_transition) {

}

double TransitionQuintic::getSimpleY(double x) {
    return transition->getChangeY()*x*x*x*(10+x*(-15+x*6))+transition->getStartY();
}

double TransitionQuintic::getDoubleBump1Y(double x) {
    double arg1 = -5;
    double root = -sqrt(9+fabs(arg1/10)*(-16+16*fabs(arg1/10)));
    double max = 0.01728+0.00576*root + fabs(arg1/10)*(-0.0288-0.00448*root + fabs(arg1/10)*(0.0032-0.00576*root + fabs(arg1/10)*(-0.0704+0.02048*root + fabs(arg1/10)*(0.1024-0.01024*root + arg1/10*0.04096))));
    return transition->getStartY() + (transition->getChangeY()/max*x*x*(x-1)*(x-1)*(x+arg1/10));
}

double TransitionQuintic::getDoubleBump2Y(double x) {
    double arg1 = 5;
    double root = sqrt(9+arg1/10*(-16+16*arg1/10));
    double max = 0.01728+0.00576*root + arg1/10*(-0.0288-0.00448*root + arg1/10*(0.0032-0.00576*root + arg1/10*(-0.0704+0.02048*root + arg1/10*(0.1024-0.01024*root - arg1/10*0.04096))));
    return transition->getStartY() + (transition->getChangeY()/max*x*x*(x-1)*(x-1)*(x-arg1/10));
}

double TransitionQuintic::getY(double x) {
    x = 1 * x / transition->getChangeX();
    x = transition->sinFuncB(x, transition->getCenter());
    x = transition->sinFuncA(x, transition->getTension());

    double func = 0.0f;
    if(getMode() == 0) func = getSimpleY(x);
    if(getMode() == 1) func = getDoubleBump1Y(x);
    if(getMode() == 2) func = getDoubleBump2Y(x);
    return lastY = func;
}


std::vector<QString> TransitionQuintic::getModes() {
    std::vector<QString> retVals;
    retVals.push_back(QString("Simple"));
    retVals.push_back(QString("Double bump 1"));
    retVals.push_back(QString("Double bump 2"));
    return retVals;
}
