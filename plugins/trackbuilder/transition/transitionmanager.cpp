#include "transition.h"
#include "transitionmanager.h"
#include "transitionbasic.h"
#include "transitionbump.h"
#include "transitionlinear.h"
#include "transitionquintic.h"
#include "transitiongeneric.h"

#include <common/global.h>
#include <QDebug>

TransitionManager::TransitionManager() {
    paramStartX = 0.0f;
    paramStartY = 1.0f;
    paramChangeX = 2.0f;
    paramChangeY = 3.0f;

    tension = 0.0f;
    center = 0.0f;

    hasPrevTransition = false;
    hasNextTransition = false;

    transition = new TransitionBasic(this);
    setTransitionType(TransitionManager::GenericTransition);
}

void TransitionManager::setTransitionType(TransitionManager::Type TransitionType) {
    delete transition;

    Transition *tmpTransition;
    transitionType = TransitionType;

    switch(TransitionType) {
    case TransitionManager::BasicTransition:
        tmpTransition = new TransitionBasic(this);
        break;
    case TransitionManager::BumpTransition:
        tmpTransition = new TransitionBump(this);
        break;
    case TransitionManager::LinearTransition:
        tmpTransition = new TransitionLinear(this);
        break;
    case TransitionManager::QuinticTransition:
        tmpTransition = new TransitionQuintic(this);
        break;
    case TransitionManager::GenericTransition:
        tmpTransition = new TransitionGeneric(this);
        break;
    }

    transition = tmpTransition;
}

std::vector<QString> TransitionManager::getModes() {
    return transition->getModes();
}


void TransitionManager::setMode(int _mode) {
    transition->setMode(_mode);
}

int TransitionManager::getMode() {
    return transition->getMode();
}

double TransitionManager::getY(double x) {
    if(x > getStartX() + getChangeX()) x = getStartX() + getChangeX();
    if(x < 0) x = 0;

    x = x - getStartX();

    return transition->getY(x);
}

double TransitionManager::getDerivative(double x) {
    if(NoLimitsRenderer::isZero(x)) {
        double y1 = getY(x)*10;
        double y2 = getY(x+0.0001f)*10;
        return (y2-y1)/((x+0.0001f)-x);
    } else {
        double y1 = getY(x)*10;
        double y2 = getY(x-0.0001f)*10;
        return ((y2-y1)/((x-0.0001f)-x));
    }
}


double TransitionManager::getLastY() {
    return transition->getLastY();
}

double TransitionManager::sinFuncA(double a, double b) {
    if (NoLimitsRenderer::isZero(b)) return a;

    double num = a;
    for (int i = 0; i < 3; i++) {
        double num2 = (num + ((b / (M_PI*2) ) * sin((M_PI*2) * num))) - a;
        double num3 = 1.0 + (b * cos((M_PI*2) * num));
        num -= num2 / num3;
    }
    return num;
}

double TransitionManager::sinFuncB(double a, double b) {
    if (NoLimitsRenderer::isZero(b)) return a;

    double num = a;
    for (int i = 0; i < 3; i++) {
        double num2 = (num + ((b / M_PI) * sin(M_PI * num))) - a;
        double num3 = 1.0 + (b * cos(M_PI * num));
        num -= num2 / num3;
    }
    return num;
}

double TransitionManager::getTension() {
    return tension;
}

double TransitionManager::getCenter() {
    return center;
}

double TransitionManager::setTension(double _tension) {
    tension = _tension * (1.0f/75.0f);
}

double TransitionManager::setCenter(double _center) {
    center = _center * (1.0f/66.7f);
}

Transition *TransitionManager::getTransition() {
    return transition;
}
