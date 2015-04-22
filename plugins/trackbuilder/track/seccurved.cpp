#include "seccurved.h"
#include "mnode.h"
#include <cmath>

#include <qWidgets/qfvdcurve.h>
#include <qWidgets/qpointplot.h>
#include <qWidgets/qcustomplot.h>

#include "trackbuilder.h"

seccurved::seccurved()
{
}

seccurved::seccurved(track* getParent, mnode* first, float getAngle, float getRadius): section(getParent, curved, first)
{
    this->fAngle = getAngle;
    this->fRadius = getRadius;
    this->fLeadIn = fAngle/3.f > 10.f ? 10.f : fAngle/3.f;
    this->fLeadOut = fAngle/3.f > 10.f ? 10.f : fAngle/3.f;
    this->fDirection = 90.0;
    this->length = 0.0;
    this->bOrientation = 0;
    this->bArgument = TIME;
    bSpeed = 0;
    fVel = 10;

    sName = "Curved Section";

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
    CurveRoll->addPoint(new qFVDCurvePoint(90.0, 0.0));
    CurveRoll->setTimeRange(0, 90);
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

void seccurved::changecurve(float newAngle, float newRadius, float newDirection)
{
    this->fAngle = newAngle;
    this->fRadius = newRadius;
    this->fDirection = newDirection;
    this->length = 0.0;
    this->updateSection();
}

void seccurved::updateSection(int node)
{
    //this->rollFunc->setMaxArgument(fAngle);

    this->length = 0.0;
    int numNodes = 1;
    float fRiddenAngle = 0.0;
    float artificialRoll = 0.0;

    valuesRollX.clear();
    valuesRollY.clear();
    valuesRollDerivativesX.clear();
    valuesRollDerivativesY.clear();
    valuesVerticalForceX.clear();
    valuesVerticalForceY.clear();
    valuesLateralForceX.clear();
    valuesLateralForceY.clear();

    fAngle = getMaxArgument();

    while(lNodes.size() > 1)
    {
        if(lNodes.size() > 2 || this->parent->lSections.at(this->parent->lSections.size()-1) == this)
        {
            delete lNodes.at(1);
        }
        lNodes.removeAt(1);
        lAngles.removeAt(1);
    }

    int sizediff = lNodes.size() - lAngles.size();
    for(int i = 0; i <= sizediff; ++i)
    {
        lAngles.append(0.f);
    }
    lAngles[0] = 0.f;
    lNodes.at(0)->updateNorm();

    float diff = lNodes.at(0)->fRollSpeed; // - rollFunc->funcList.at(0)->startValue;
    CurveRoll->modifyPoint(0, qFVDCurvePoint(0.0, diff / (180/M_PI)), false);

    /*if(bOrientation == 1)
    {
        diff += glm::dot(lNodes[0]->vDir, glm::vec3(0.f, 1.f, 0.f))*lNodes[0]->getYawChange();
    }
    rollFunc->funcList.at(0)->translateValues(diff);
    rollFunc->translateValues(rollFunc->funcList.at(0));
*/

    plotAdditionalGraphes(fRiddenAngle, lNodes.at(0));

    while(fRiddenAngle < this->fAngle - std::numeric_limits<float>::epsilon())
    {
        lNodes.append(new mnode(*(this->lNodes.at(numNodes-1))));

        if(lNodes.at(numNodes)->fVel < 0.1f)
        {
            qDebug("warning: train goes very slowly");
            break;
        }

        float deltaAngle, fVirtualRadius;
        if(fRiddenAngle < fLeadIn)
        {
            fVirtualRadius = this->fRadius*(fLeadIn+1)/(fRiddenAngle+1);
            deltaAngle = this->lNodes.at(numNodes)->fVel / fVirtualRadius / 1000.f * 180/M_PI;
        }
        else if(fRiddenAngle > fAngle-fLeadOut)
        {
            fVirtualRadius =this->fRadius*(fLeadOut+1)/(fAngle-fRiddenAngle+1);
            deltaAngle = this->lNodes.at(numNodes)->fVel / fVirtualRadius / 1000.f * 180/M_PI;
        }
        else
        {
            fVirtualRadius = this->fRadius;
            deltaAngle = this->lNodes.at(numNodes)->fVel / fVirtualRadius / 1000.f * 180/M_PI;
        }

        fRiddenAngle += deltaAngle;
        lAngles.append(fRiddenAngle);


        lNodes.at(numNodes)->updateNorm();

        float fPureDirection = fDirection - artificialRoll; //- lNodes.at(numNodes)->fRoll;

        lNodes.at(numNodes)->vDir = glm::vec3(glm::rotate(deltaAngle, (float)glm::cos(-fPureDirection*M_PI/180) * lNodes.at(numNodes-1)->vLat  + (float)glm::sin(-fPureDirection*M_PI/180) * lNodes.at(numNodes-1)->vNorm)*glm::vec4(lNodes.at(numNodes-1)->vDir, 0));
        lNodes.at(numNodes)->vLat = glm::vec3(glm::rotate(deltaAngle, (float)glm::cos(-fPureDirection*M_PI/180) * lNodes.at(numNodes-1)->vLat  + (float)glm::sin(-fPureDirection*M_PI/180) * lNodes.at(numNodes-1)->vNorm)*glm::vec4(lNodes.at(numNodes-1)->vLat, 0));
        lNodes.at(numNodes)->vDir = glm::normalize(lNodes.at(numNodes)->vDir);
        lNodes.at(numNodes)->vLat = glm::normalize(lNodes.at(numNodes)->vLat);

        lNodes.at(numNodes)->vPos += lNodes.at(numNodes)->fVel / 2000.f * lNodes.at(numNodes)->vDir + lNodes.at(numNodes-1)->fVel / 2000.f * lNodes.at(numNodes-1)->vDir;

        lNodes.at(numNodes)->updateNorm();

        lNodes.at(numNodes)->setRoll((CurveRoll->getEulerY(fRiddenAngle) * (180/M_PI))/1000.f); //rollFunc->getValue((float)(i+1)/numNodes*fAngle)); //360./numNodes*(i+1));

        if(bOrientation == 1)
        {
            calcDirFromLast(numNodes);
            lNodes[numNodes]->setRoll(glm::dot(lNodes[numNodes]->vDir, glm::vec3(0.f, -1.f, 0.f))*lNodes[numNodes]->fYawFromLast);
            artificialRoll += glm::dot(lNodes[numNodes]->vDir, glm::vec3(0.f, -1.f, 0.f))*lNodes[numNodes]->fYawFromLast;
        }

        artificialRoll += (CurveRoll->getEulerY(fRiddenAngle) * (180/M_PI))/1000.f;
        lNodes.at(numNodes)->updateNorm();

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


        //lNodes.at(numNodes)->vPos = glm::vec3(glm::translate(glm::rotate(glm::translate(vCenter), this->fAngle/numNodes, (float)glm::cos(-fDirection*M_PI/180) * lNodes.at(numNodes)->vLat  + (float)glm::sin(-fDirection*M_PI/180) * lNodes.at(numNodes-1)->vNorm), -vCenter)*glm::vec4(lNodes.at(numNodes-1)->vPos, 1));

        lNodes.at(numNodes)->updateRoll();

        lNodes.at(numNodes)->fDistFromLast = glm::distance(lNodes.at(numNodes)->fPosHeart(parent->fHeart), lNodes.at(numNodes-1)->fPosHeart(parent->fHeart));
        lNodes.at(numNodes)->fTotalLength += lNodes.at(numNodes)->fDistFromLast;
        lNodes.at(numNodes)->fHeartDistFromLast = glm::distance(lNodes.at(numNodes)->vPos, lNodes.at(numNodes-1)->vPos);
        lNodes.at(numNodes)->fTotalHeartLength += lNodes.at(numNodes)->fHeartDistFromLast;

        lNodes.at(numNodes)->fRollSpeed = (CurveRoll->getEulerY(fRiddenAngle) * (180/M_PI)); /* /1000.f/lNodes.at(numNodes)->fDistFromLast; */
        if(bOrientation == 1)
        {
            lNodes.at(numNodes)->fRollSpeed += glm::dot(lNodes[numNodes]->vDir, glm::vec3(0.f, -1.f, 0.f))*lNodes[numNodes]->fYawFromLast*1000.f;
        }


        calcDirFromLast(numNodes);

        float temp = cos(fabs(lNodes[numNodes]->getPitch())*M_PI/180.f);
        float forceAngle = sqrt(temp*temp*lNodes.at(numNodes)->fYawFromLast*lNodes.at(numNodes)->fYawFromLast + lNodes.at(numNodes)->fPitchFromLast*lNodes.at(numNodes)->fPitchFromLast);//deltaAngle;
        lNodes.at(numNodes)->fAngleFromLast = forceAngle;

        glm::vec3 forceVec = glm::vec3(0.f, 1.f, 0.f) + (float)((lNodes.at(numNodes)->fVel*lNodes.at(numNodes)->fVel) / (9.80665 * lNodes.at(numNodes)->fVel/forceAngle * 0.18f/M_PI)) * glm::normalize(glm::vec3(glm::rotate(lNodes[numNodes]->fDirFromLast/*fDirection - lNodes.at(numNodes)->fRoll*/, -lNodes.at(numNodes)->vDir)*glm::vec4(-lNodes.at(numNodes)->vNorm, 0.f)));

        lNodes.at(numNodes)->forceNormal = - glm::dot(forceVec, glm::normalize(lNodes.at(numNodes)->vNorm));
        lNodes.at(numNodes)->forceLateral = - glm::dot(forceVec, glm::normalize(lNodes.at(numNodes)->vLat));


        this->length += lNodes.at(numNodes)->fDistFromLast;


        plotAdditionalGraphes(fRiddenAngle, lNodes.at(numNodes));
        numNodes++;
    }

    updateGraphStates();
}

double seccurved::getMaxArgument()
{
    return CurveRoll->points[CurveRoll->points.size()-1]->x;
}


void seccurved::plotAdditionalGraphes(float x, mnode *lNode) {
    valuesRollX.append(x);
    valuesRollY.append((lNode->fRoll / (180/M_PI)) * 2);

    valuesRollDerivativesX.append(x);
    valuesRollDerivativesY.append(CurveRoll->getQuaternionDerivative(x));

    valuesVerticalForceX.append(x);
    valuesVerticalForceY.append(lNode->forceNormal);

    valuesLateralForceX.append(x);
    valuesLateralForceY.append(lNode->forceLateral);
}

void seccurved::updateGraphStates() {
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

