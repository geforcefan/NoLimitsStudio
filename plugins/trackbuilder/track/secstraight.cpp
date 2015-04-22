#include "secstraight.h"
#include "mnode.h"

#include <cmath>
#include <QDebug>

#include <qWidgets/qfvdcurve.h>
#include <qWidgets/qpointplot.h>
#include <qWidgets/qcustomplot.h>

#include "trackbuilder.h"

using namespace std;

secstraight::secstraight() : section()
{
}

secstraight::secstraight(track* getParent, mnode* first, float getlength) : section(getParent, straight, first)
{
    this->fHLength = getlength;
    this->bArgument = TIME;
    this->bOrientation = QUATERNION;
    this->useStartSpeed = false;

    bSpeed = 1;
    fVel = 20;
    sName = "Straight Section";

    // Additional Graphes
    this->showRoll = true;
    this->showShowRollDerivative = false;
    this->showVerticalForce = true;
    this->showLateralForce = true;

    // Roll
    CurveRoll = new qFVDCurve();
    CurveRoll->setYLabel(QString("Roll"));
    CurveRoll->setColor(QColor(255, 0, 0, 255));
    CurveRoll->setYRange(-360*1*(M_PI/180), 360*1*(M_PI/180));
    CurveRoll->addPoint(new qFVDCurvePoint(0.0, 0.0));
    CurveRoll->addPoint(new qFVDCurvePoint(20.0, 0.0));
    CurveRoll->setTimeRange(0, 20);
    CurveRoll->init();
    CurveRoll->enableTime();

    // Resulting Banking
    CurveRoll->pointPlot->plotter->addGraph(CurveRoll->pointPlot->plotter->xAxis, CurveRoll->pointPlot->plotter->yAxis);
    CurveRoll->pointPlot->plotter->graph(1)->setPen(QPen(QColor(0, 80, 0, 180), 1));
    CurveRoll->pointPlot->plotter->graph(1)->setBrush(QBrush(QColor(0, 80, 0, 20)));

    CurveRoll->pointPlot->plotter->yAxis2->setRange(-5, 5);
    CurveRoll->pointPlot->plotter->yAxis2->setLabel("Forces");
    CurveRoll->pointPlot->plotter->yAxis2->setVisible(true);

    // Resulting Derivative
    CurveRoll->pointPlot->plotter->addGraph(CurveRoll->pointPlot->plotter->xAxis, CurveRoll->pointPlot->plotter->yAxis);
    CurveRoll->pointPlot->plotter->graph(2)->setPen(QPen(QColor(255, 0, 0, 255), 1, Qt::DashLine));

    // Resulting Vertical Force
    CurveRoll->pointPlot->plotter->addGraph(CurveRoll->pointPlot->plotter->xAxis, CurveRoll->pointPlot->plotter->yAxis2);
    CurveRoll->pointPlot->plotter->graph(3)->setPen(QPen(QColor(0, 0, 100, 255), 2));
    CurveRoll->pointPlot->plotter->graph(3)->setBrush(QBrush(QColor(0, 0, 100, 20)));

    // Resulting Lateral Force
    CurveRoll->pointPlot->plotter->addGraph(CurveRoll->pointPlot->plotter->xAxis, CurveRoll->pointPlot->plotter->yAxis2);
    CurveRoll->pointPlot->plotter->graph(4)->setPen(QPen(QColor(0, 100, 0, 255), 2));
    CurveRoll->pointPlot->plotter->graph(4)->setBrush(QBrush(QColor(0, 100, 0, 20)));
}

void secstraight::changelength(float newlength)
{
    this->fHLength = newlength;
    this->updateSection();
}

void secstraight::updateSection(int node)
{
    int numNodes = 1;
    this->length = 0;
    fHLength = getMaxArgument();

    valuesRollX.clear();
    valuesRollY.clear();
    valuesRollDerivativesX.clear();
    valuesRollDerivativesY.clear();
    valuesVerticalForceX.clear();
    valuesVerticalForceY.clear();
    valuesLateralForceX.clear();
    valuesLateralForceY.clear();

    while(lNodes.size() > 1)
    {
        if(lNodes.size() > 2 || this->parent->lSections.at(this->parent->lSections.size()-1) == this)
        {
            delete lNodes.at(1);
        }
        lNodes.removeAt(1);
    }
    lNodes.at(0)->updateNorm();

    float diff = lNodes.at(0)->fRollSpeed;

    CurveRoll->modifyPoint(0, qFVDCurvePoint(0.0, diff / (180/M_PI)), false);

    bool lastNode = false;

    float fCurLength = 0.0f;

    //lNodes.replace(0, new mnode(*(lNodes.at(0))));

    //if(useStartSpeed) lNodes.at(0)->fVel = this->fVel;

    //qDebug () << lNodes.at(0)->fVel << useStartSpeed;

    plotAdditionalGraphes(fCurLength, lNodes.at(0));

    while(fCurLength < this->fHLength - std::numeric_limits<float>::epsilon() && !lastNode)
    {
        //qDebug() << "fCurLength" << fCurLength;

        //qDebug() << CurveRoll->getEulerY(fCurLength);

        lNodes.append(new mnode(*(this->lNodes.at(numNodes-1))));

        float dTime;
        if(lNodes.at(numNodes)->fVel < 0.1f)
        {
            //qDebug("warning: train goes very slowly");
            break;
        }
        if(lNodes.at(numNodes)->fVel/1000.f < this->fHLength - fCurLength)
        {
            dTime = 1000.f;
        }
        else
        {
            lastNode = true;
            dTime = (lNodes.at(numNodes)->fVel + std::numeric_limits<float>::epsilon())/(this->fHLength - fCurLength);
        }

        lNodes.at(numNodes)->vPos += lNodes.at(numNodes)->vDir*(lNodes.at(numNodes)->fVel/dTime);

        fCurLength += lNodes.at(numNodes)->fVel/dTime;

        lNodes.at(numNodes)->setRoll((CurveRoll->getEulerY(fCurLength) * (180/M_PI)) /dTime); //rollFunc->getValue((i+1)/10.0) - rollFunc->getValue(i/10.0));

        lNodes.at(numNodes)->updateNorm();

        lNodes.at(numNodes)->forceNormal = -lNodes.at(numNodes)->vNorm.y;
        lNodes.at(numNodes)->forceLateral = -lNodes.at(numNodes)->vLat.y;

        lNodes.at(numNodes)->fDistFromLast = glm::distance(lNodes.at(numNodes)->fPosHeart(parent->fHeart), lNodes.at(numNodes-1)->fPosHeart(parent->fHeart));
        lNodes.at(numNodes)->fTotalLength += lNodes.at(numNodes)->fDistFromLast;
        lNodes.at(numNodes)->fHeartDistFromLast = glm::distance(lNodes.at(numNodes)->vPos, lNodes.at(numNodes-1)->vPos);
        lNodes.at(numNodes)->fTotalHeartLength += lNodes.at(numNodes)->fHeartDistFromLast;

        lNodes.at(numNodes)->fRollSpeed = (CurveRoll->getEulerY(fCurLength) * (180/M_PI));

        lNodes.at(numNodes)->fAngleFromLast = 0.0;
        calcDirFromLast(numNodes);

        if(bSpeed)
        {
            float heightDiff = lNodes.at(numNodes)->fPosHeart(parent->fHeart).y - lNodes.at(numNodes-1)->fPosHeart(parent->fHeart).y;
            lNodes.at(numNodes)->fVel = glm::sqrt(lNodes.at(numNodes-1)->fVel*lNodes.at(numNodes-1)->fVel - 2*9.08665*heightDiff);
            lNodes.at(numNodes)->fVel = glm::sqrt(lNodes.at(numNodes)->fVel * lNodes.at(numNodes)->fVel - 2*lNodes.at(numNodes)->fDistFromLast*parent->fFriction);
        }
        else
        {
            lNodes.at(numNodes)->fVel = this->fVel;
        }

        lNodes.at(numNodes)->fTime = (lNodes.size()-1)/1000.0f;
        this->length += lNodes.at(numNodes)->fDistFromLast;

        plotAdditionalGraphes(fCurLength, lNodes.at(numNodes));
        ++numNodes;
    }

    updateGraphStates();

    /*qDebug() << "elapsed time" << QString().number((lNodes.size()-1)/1000.0f, 'f', 3).append(" s");
    qDebug() << "elapsed length" << this->length;*/
}

double secstraight::getMaxArgument()
{
    return CurveRoll->points[CurveRoll->points.size()-1]->x;
}

void secstraight::plotAdditionalGraphes(float x, mnode *lNode) {
    valuesRollX.append(x);
    valuesRollY.append((lNode->fRoll / (180/M_PI)) * 2);

    valuesRollDerivativesX.append(x);
    valuesRollDerivativesY.append(CurveRoll->getQuaternionDerivative(x));

    valuesVerticalForceX.append(x);
    valuesVerticalForceY.append(lNode->forceNormal);

    valuesLateralForceX.append(x);
    valuesLateralForceY.append(lNode->forceLateral);
}

void secstraight::updateGraphStates() {
    CurveRoll->pointPlot->Mutex->lock();

    CurveRoll->pointPlot->plotter->graph(1)->setVisible(showRoll);
    CurveRoll->pointPlot->plotter->graph(1)->setData(valuesRollX, valuesRollY);

    CurveRoll->pointPlot->plotter->graph(2)->setVisible(showShowRollDerivative);
    CurveRoll->pointPlot->plotter->graph(2)->setData(valuesRollDerivativesX, valuesRollDerivativesY);

    CurveRoll->pointPlot->plotter->graph(3)->setVisible(showVerticalForce);
    CurveRoll->pointPlot->plotter->graph(3)->setData(valuesVerticalForceX, valuesVerticalForceY);

    CurveRoll->pointPlot->plotter->graph(4)->setVisible(showLateralForce);
    CurveRoll->pointPlot->plotter->graph(4)->setData(valuesLateralForceX, valuesLateralForceY);

    CurveRoll->pointPlot->Mutex->unlock();

    trackBuilder->graphChanges = true;
}
