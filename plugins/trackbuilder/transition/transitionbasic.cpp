#include "transitionbasic.h"
#include "transitionmanager.h"
#include <math.h>

TransitionBasic::TransitionBasic(TransitionManager *_transition) : Transition(_transition) {

}

double TransitionBasic::getY(double x) {
    double num3 = 1 * x / transition->getChangeX();
    double num2 = transition->sinFuncB(num3, transition->getCenter());
    double num = transition->sinFuncA(num2, transition->getTension());

    num2 = ((num * num) * num) * ((num * ((6 * num) - 15)) + 10);
    double func = transition->getStartY() + (num2 * transition->getChangeY());
    return lastY = func;
}

std::vector<QString> TransitionBasic::getModes() {
    std::vector<QString> retVals;
    /*retVals.push_back(QString("Basic1"));
    retVals.push_back(QString("Basic2"));
    retVals.push_back(QString("Basic3"));*/
    return retVals;
}
