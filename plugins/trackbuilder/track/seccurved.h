#ifndef SECCURVED_H
#define SECCURVED_H

#include "section.h"
#include "track.h"

#include <QVector>

class qFVDCurve;

class seccurved : public section
{
public:
    seccurved();
    seccurved(track* getParent, mnode* first, float getAngle, float getRadius);
    void changecurve(float newAngle, float newRadius, float newDirection);
    virtual void updateSection(int node = 0);
    virtual double getMaxArgument();

    void plotAdditionalGraphes(float x, mnode *lNode);
    void updateGraphStates();

    qFVDCurve *CurveRoll;

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
private:
    QList<float> lAngles;
};

#endif // SECCURVED_H
