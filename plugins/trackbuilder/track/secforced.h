#ifndef SECFORCED_H
#define SECFORCED_H

#include "track.h"
#include "section.h"

#include <QVector>

class qFVDCurve;

class secforced : public section
{
public:
    secforced();
    ~secforced();
    secforced(track* getParent, mnode* first, float getlength = 10.0);
    virtual void updateSection(int node = 0);
    void updateDistanceSection(int node = 0);
    virtual double getMaxArgument();

    void plotAdditionalGraphes(float x, mnode *lNode);
    void updateGraphStates();

    qFVDCurve *CurveRoll;
    qFVDCurve *CurveVert;
    qFVDCurve *CurveLat;

    bool showRoll;
    bool showShowRollDerivative;

    QVector<double> valuesRollX;
    QVector<double> valuesRollY;

    QVector<double> valuesRollDerivativesX;
    QVector<double> valuesRollDerivativesY;
};

#endif // SECFORCED_H
