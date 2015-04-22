#ifndef SECSTRAIGHT_H
#define SECSTRAIGHT_H

#include "section.h"
#include "track.h"

#include <QVector>

class qFVDCurve;

class secstraight : public section
{
public:
    secstraight();
    secstraight(track* getParent, mnode* first, float getlength = 10.0);
    void changelength(float newlength);
    virtual void updateSection(int node = 0);
    virtual double getMaxArgument();

    void plotAdditionalGraphes(float x, mnode *lNode);
    void updateGraphStates();

    qFVDCurve *CurveRoll;

    bool useStartSpeed;

    bool showRoll;
    bool showShowRollDerivative;
    bool showVerticalForce;
    bool showLateralForce;

    QVector<double> valuesRollX;
    QVector<double> valuesRollY;

    QVector<double> valuesRollDerivativesX;
    QVector<double> valuesRollDerivativesY;

    QVector<double> valuesVerticalForceX;
    QVector<double> valuesVerticalForceY;

    QVector<double> valuesLateralForceX;
    QVector<double> valuesLateralForceY;
};

#endif // SECSTRAIGHT_H
