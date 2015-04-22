#include "transitiongeneric.h"
#include "transitionmanager.h"
#include <math.h>

#include <QDebug>

TransitionGeneric::TransitionGeneric(TransitionManager *_transition) : Transition(_transition) {
    slope = 0.0f;
    useSlope = false;

    shape1 = 0.0f;
    shape2 = 0.0f;
}

double TransitionGeneric::getY(double x) {
    double slope1 = 0.0f;
    double slope2 = 0.0f;

    double np_shape1 = shape1;
    double np_shape2 = shape2;

    double cp_shape1 = 0.0f;
    double cp_shape2 = 0.0f;

    if(transition->hasPrevTransition) slope1 = transition->prevTransition->getDerivative(transition->prevTransition->getStartX() + transition->prevTransition->getChangeX()) / 10;
    if(transition->hasNextTransition && transition->nextTransition->getTransitionType() != TransitionManager::GenericTransition) {
        slope2 = transition->nextTransition->getDerivative(transition->nextTransition->getStartX()) / 10;
    }
    if(transition->hasPrevTransition && transition->prevTransition->getTransitionType() == TransitionManager::GenericTransition) {
        TransitionGeneric *prevTransitionGeneric = static_cast<TransitionGeneric*>(transition->prevTransition->getTransition());
        cp_shape1 = prevTransitionGeneric->shape1;
        cp_shape2 = prevTransitionGeneric->shape2;
    }

    if(useSlope) slope2 = slope;

    double dX = transition->getChangeX();
    double dY = -transition->getChangeY();

    double cof7 = (dX * (60.0f * (slope1 + slope2) + dX * (12.0f * cp_shape1 - 12.0f * np_shape1 + dX * (cp_shape2 + np_shape2))) + 120.0f * dY ) / (6.0f * pow(dX , 7.0f));
    double cof6 = (dX * (216.0f * slope1 + 204.0f * slope2 +
                            dX * (45.0f * cp_shape1 - 39.0f * np_shape1 +
                            4.0f * cp_shape2 * dX + 3.0f * np_shape2 * dX)) +
                            420.0f * (dY))/(-6.0f* pow(dX, 6.0f));
    double cof5 = (dX *(90.0f *slope1+78.0f* slope2 + dX *(20.0f* cp_shape1 - 14.0f *np_shape1 + 2.0f *cp_shape2 *dX + np_shape2 * dX)) + 168.0f *dY) / (2.0f * pow(dX, 5.0f));
    double cof4 = ( dX *(30.0f* (4.0f* slope1+3* slope2) + dX *(30.0f* cp_shape1 - 15.0f* np_shape1 + 4.0f *cp_shape2 * dX + np_shape2 *dX)) + 210.0f * dY) / (-6.0f * pow(dX,  4.0f));
    double cof3 =  cp_shape2 / 6.0f;
    double cof2 = cp_shape1 / 2.0f;

    double s = x;
    double ty = transition->getStartY() + s * (slope1 + s * (cof2 + s * (cof3 + s * (cof4 + s * (cof5 + s * (cof6 + cof7 * s))))));

    return lastY = ty;
}


std::vector<QString> TransitionGeneric::getModes() {
    std::vector<QString> retVals;
    return retVals;
}
