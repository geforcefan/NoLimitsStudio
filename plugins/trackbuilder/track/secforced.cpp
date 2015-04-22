#include "secforced.h"
#include "mnode.h"

#include <qWidgets/qfvdcurve.h>
#include <qWidgets/qpointplot.h>
#include <qWidgets/qcustomplot.h>

#include "trackbuilder.h"

secforced::secforced()
{
}

secforced::~secforced()
{
}

secforced::secforced(track* getParent, mnode* first, float gettime): section(getParent, forced, first)
{
    this->iTime = (int)(gettime+0.5);
    this->length = 0.0;

    sName = "Forced Section";

    // Additional Graphes
    this->showRoll = true;
    this->showShowRollDerivative = false;

    // Roll
    CurveRoll = new qFVDCurve();
    CurveRoll->setYLabel(QString("Roll"));
    CurveRoll->setColor(QColor(255, 0, 0, 255));
    CurveRoll->setYRange(-360*1*(M_PI/180), 360*1*(M_PI/180));
    CurveRoll->addPoint(new qFVDCurvePoint(0.0, 0.0));
    CurveRoll->addPoint(new qFVDCurvePoint(2.0, 0.0));
    CurveRoll->setTimeRange(0, 5);
    CurveRoll->init();
    CurveRoll->enableTime();

    // Resulting Banking
    CurveRoll->pointPlot->plotter->addGraph(CurveRoll->pointPlot->plotter->xAxis, CurveRoll->pointPlot->plotter->yAxis);
    CurveRoll->pointPlot->plotter->graph(1)->setPen(QPen(QColor(0, 80, 0, 180), 1));
    CurveRoll->pointPlot->plotter->graph(1)->setBrush(QBrush(QColor(0, 80, 0, 20)));

    // Resulting Derivative
    CurveRoll->pointPlot->plotter->addGraph(CurveRoll->pointPlot->plotter->xAxis, CurveRoll->pointPlot->plotter->yAxis);
    CurveRoll->pointPlot->plotter->graph(2)->setPen(QPen(QColor(255, 0, 0, 255), 1, Qt::DashLine));

    // Vert
    CurveVert = new qFVDCurve();
    CurveVert->setYLabel(QString("Vertical Force"));
    CurveVert->setColor(QColor(0, 0, 255, 255));
    CurveVert->setYRange(-5, 5);
    CurveVert->addPoint(new qFVDCurvePoint(0.0, 1.0));
    CurveVert->addPoint(new qFVDCurvePoint(2.0, 1.0));
    CurveVert->setTimeRange(0, 5);
    CurveVert->init();
    CurveVert->enableTime();

    // Lat
    CurveLat = new qFVDCurve();
    CurveLat->setYLabel(QString("Lateral Force"));
    CurveLat->setColor(QColor(0, 0, 255, 255));
    CurveLat->setYRange(-2, 2);
    CurveLat->addPoint(new qFVDCurvePoint(0.0, 0.0));
    CurveLat->setTimeRange(0, 5);
    CurveLat->init();
    CurveLat->enableTime();

    this->bOrientation = QUATERNION;
    this->bArgument = TIME;
    bSpeed = 1;
    fVel = 10;
}

void secforced::updateSection(int node)
{
    valuesRollX.clear();
    valuesRollY.clear();
    valuesRollDerivativesX.clear();
    valuesRollDerivativesY.clear();

    /*if(bArgument == DISTANCE)
    {
        updateDistanceSection(node);
        return;
    }*/

    node = node > lNodes.size()-2 ? lNodes.size()-2 : node;
    node = node < 0 ? 0 : node;

    int numNodes = (int)(getMaxArgument()*1000+0.5);
    iTime = numNodes;
    this->length = 0;

    if(node >= lNodes.size()-1 && node > 0) node = lNodes.size()-2;

    while(lNodes.size() > 1+node)
    {
        if(lNodes.size() > 2+node || this->parent->lSections.at(this->parent->lSections.size()-1) == this)
        {
            delete lNodes.at(1+node);
        }
        lNodes.removeAt(1+node);
    }

    if(node == 0)
    {
        lNodes.at(0)->updateNorm();

        float diff = lNodes.at(0)->forceNormal;
        if(diff != diff)
        {
            lNodes.append(new mnode(*(this->lNodes.at(0))));
            return;
        }
        CurveVert->modifyPoint(0, qFVDCurvePoint(0.0, diff), false);

        diff = lNodes.at(0)->forceLateral;
        if(diff != diff)
        {
            lNodes.append(new mnode(*(this->lNodes.at(0))));
            return;
        }
        CurveLat->modifyPoint(0, qFVDCurvePoint(0.0, diff), false);

        diff = lNodes.at(0)->fRollSpeed;
        if(bOrientation == 1)
        {
            diff += glm::dot(lNodes[0]->vDir, glm::vec3(0.f, 1.f, 0.f))*lNodes[0]->getYawChange();
        }
        CurveRoll->modifyPoint(0, qFVDCurvePoint(0.0, diff / (180/M_PI)), false);
    }

    plotAdditionalGraphes(0, lNodes.at(0));

    for(int i = node; i < numNodes; i++)
    {
        lNodes.append(new mnode(*(this->lNodes.at(i))));

        glm::vec3 forceVec = - CurveVert->getEulerY((i+1)/1000.f) * lNodes.at(i+1)->vNorm - CurveLat->getEulerY((i+1)/1000.f) * lNodes.at(i+1)->vLat - glm::vec3(0.f, 1.f, 0.f);

        lNodes.at(i+1)->forceNormal = CurveVert->getEulerY((i+1)/1000.f);
        lNodes.at(i+1)->forceLateral = CurveLat->getEulerY((i+1)/1000.f);

        float nForce = - glm::dot(forceVec, glm::normalize(lNodes.at(i+1)->vNorm))*9.80665*180/M_PI;
        float lForce = - glm::dot(forceVec, glm::normalize(lNodes.at(i+1)->vLat))*9.80665*180/M_PI;

        lNodes.at(i+1)->vDir = glm::normalize(glm::vec3(glm::rotate(nForce/lNodes.at(i)->fVel/1000.f, lNodes.at(i)->vLat) * glm::rotate(-lForce/lNodes.at(i)->fVel/1000.f, lNodes.at(i)->vNorm) * glm::vec4(lNodes.at(i)->vDir, 0.f)));
        lNodes.at(i+1)->vLat = glm::normalize(glm::vec3(glm::rotate(-lForce/lNodes.at(i)->fVel/1000.f, lNodes.at(i)->vNorm) * glm::vec4(lNodes.at(i)->vLat, 0.f)));

        lNodes.at(i+1)->vPos += lNodes.at(i+1)->vDir*(lNodes.at(i+1)->fVel/2000.f) + lNodes.at(i)->vDir*(lNodes.at(i+1)->fVel/2000.f);

        lNodes.at(i+1)->updateNorm();

        lNodes.at(i+1)->setRoll((CurveRoll->getEulerY((i+1)/1000.f) * (180/M_PI)) /1000.f); // - CurveRoll->getEulerY(i/1000.f));

        if(bOrientation == 1)
        {
            calcDirFromLast(i+1);
            lNodes[i+1]->setRoll(glm::dot(lNodes[i+1]->vDir, glm::vec3(0.f, -1.f, 0.f))*lNodes[i+1]->fYawFromLast);
        }

        lNodes.at(i+1)->updateNorm();

        lNodes.at(i+1)->fDistFromLast = glm::distance(lNodes.at(i+1)->fPosHeart(parent->fHeart), lNodes.at(i)->fPosHeart(parent->fHeart));
        lNodes.at(i+1)->fTotalLength += lNodes.at(i+1)->fDistFromLast;
        lNodes.at(i+1)->fHeartDistFromLast = 0.7f*glm::distance(lNodes.at(i+1)->vPos, lNodes.at(i)->vPos) + 0.3f*lNodes.at(i+1)->fDistFromLast;
        lNodes.at(i+1)->fTotalHeartLength += lNodes.at(i+1)->fHeartDistFromLast;
        lNodes.at(i+1)->fRollSpeed = CurveRoll->getEulerY((i+1)/1000.f) * (180/M_PI);  // /1000.f/lNodes.at(i+1)->fDistFromLast;
        if(bOrientation == 1)
        {
            lNodes.at(i+1)->fRollSpeed += glm::dot(lNodes[i+1]->vDir, glm::vec3(0.f, -1.f, 0.f))*lNodes[i+1]->fYawFromLast*1000.f;
        }


        calcDirFromLast(i+1);
        float temp = cos(fabs(lNodes[i+1]->getPitch())*M_PI/180.f);
        float forceAngle = sqrt(temp*temp*lNodes.at(i+1)->fYawFromLast*lNodes.at(i+1)->fYawFromLast + lNodes.at(i+1)->fPitchFromLast*lNodes.at(i+1)->fPitchFromLast);//deltaAngle;
        lNodes.at(i+1)->fAngleFromLast = forceAngle;

        if(bSpeed)
        {
            float heightDiff = lNodes.at(i+1)->fPosHeart(parent->fHeart).y - lNodes.at(i)->fPosHeart(parent->fHeart).y;
            lNodes.at(i+1)->fVel = glm::sqrt(lNodes.at(i)->fVel*lNodes.at(i)->fVel - 2*9.08665*heightDiff);
            lNodes.at(i+1)->fVel = glm::sqrt(lNodes.at(i+1)->fVel * lNodes.at(i+1)->fVel - 2*lNodes.at(i+1)->fDistFromLast*parent->fFriction); // glm::length(forceVec + glm::vec3(0, 1.f, 0))
        }
        else
        {
            lNodes.at(i+1)->fVel = this->fVel;
        }

        if(fabs(lNodes.at(i+1)->fAngleFromLast) < std::numeric_limits<float>::epsilon())
        {
            forceVec = glm::vec3(0.f, 1.f, 0.f);
        }
        else
        {
            forceVec = glm::vec3(0.f, 1.f, 0.f) + (float)((lNodes.at(i+1)->fVel*lNodes.at(i+1)->fVel) / (9.80665 * lNodes.at(i+1)->fVel/lNodes.at(i+1)->fAngleFromLast * 0.18f/M_PI)) * glm::normalize(glm::vec3(glm::rotate(lNodes[i+1]->fDirFromLast, -lNodes.at(i+1)->vDir)*glm::vec4(-lNodes.at(i+1)->vNorm, 0.f)));
        }
        lNodes.at(i+1)->forceNormal = - glm::dot(forceVec, glm::normalize(lNodes.at(i+1)->vNorm));
        lNodes.at(i+1)->forceLateral = - glm::dot(forceVec, glm::normalize(lNodes.at(i+1)->vLat));


        plotAdditionalGraphes((i+1)/1000.f, lNodes.at(i+1));
        this->length += lNodes.at(i+1)->fDistFromLast;
        if(i==0)
        {
            //qDebug("Flexion of first forced Node: %f", lNodes[1]->fFlexion());
        }
    }
    updateGraphStates();
}

void secforced::updateDistanceSection(int node)
{
 /*   node = node < 0 ? 0 : node;

    int i = 0;
    this->length = 0.f;
    float hDist = 0.f;
    while(hDist < (float)node/1000.f && i+1 < lNodes.size())
    {
        hDist += lNodes[++i]->fHeartDistFromLast;
        length += lNodes[i]->fDistFromLast;
    }

    if(i >= lNodes.size()-1 && i > 0) i = lNodes.size()-2;

    while(lNodes.size() > 1+i)
    {
        if(lNodes.size() > 2+i || this->parent->lSections.at(this->parent->lSections.size()-1) == this)
        {
            delete lNodes.at(1+i);
        }
        lNodes.removeAt(1+i);
    }

    if(i == 0)
    {
        lNodes.at(0)->updateNorm();

        float diff = lNodes.at(0)->forceNormal; // - normForce->funcList.at(0)->startValue;
        if(diff != diff)
        {
            lNodes.append(new mnode(*(this->lNodes.at(0))));
            return;
        }
        normForce->funcList.at(0)->translateValues(diff);
        normForce->translateValues(normForce->funcList.at(0));

        diff = lNodes.at(0)->forceLateral; // - latForce->funcList.at(0)->startValue;
        if(diff != diff)
        {
            lNodes.append(new mnode(*(this->lNodes.at(0))));
            return;
        }
        latForce->funcList.at(0)->translateValues(diff);
        latForce->translateValues(latForce->funcList.at(0));

        diff = lNodes.at(0)->fRollSpeed/lNodes[0]->fVel; // - rollFunc->funcList.at(0)->startValue;
        if(bOrientation == 1)
        {
            diff += glm::dot(lNodes[0]->vDir, glm::vec3(0.f, 1.f, 0.f))*lNodes[0]->getYawChange()/lNodes[0]->fVel;
        }
        rollFunc->funcList.at(0)->translateValues(diff);
        rollFunc->translateValues(rollFunc->funcList.at(0));
    }

    //qDebug("deleting complete");

    float end = this->getMaxArgument();

    while(hDist < end)
    {
        lNodes.append(new mnode(*(this->lNodes.at(i))));

        glm::vec3 forceVec = - CurveVert->getEulerY(hDist+lNodes[i+1]->fVel/1000.f) * lNodes.at(i+1)->vNorm - CurveLat->getEulerY(hDist+lNodes[i+1]->fVel/1000.f) * lNodes.at(i+1)->vLat - glm::vec3(0.f, 1.f, 0.f);

        lNodes.at(i+1)->forceNormal = CurveVert->getEulerY(hDist+lNodes[i+1]->fVel/1000.f);
        lNodes.at(i+1)->forceLateral = CurveLat->getEulerY(hDist+lNodes[i+1]->fVel/1000.f);

        float nForce = - glm::dot(forceVec, glm::normalize(lNodes.at(i+1)->vNorm))*9.80665*180/M_PI;
        float lForce = - glm::dot(forceVec, glm::normalize(lNodes.at(i+1)->vLat))*9.80665*180/M_PI;

        //qDebug("nForce = %f\tlForce = %f", nForce, lForce);

        lNodes.at(i+1)->vDir = glm::normalize(glm::vec3(glm::rotate(nForce/lNodes.at(i)->fVel/1000.f, lNodes.at(i)->vLat) * glm::rotate(-lForce/lNodes.at(i)->fVel/1000.f, lNodes.at(i)->vNorm) * glm::vec4(lNodes.at(i)->vDir, 0.f)));
        lNodes.at(i+1)->vLat = glm::normalize(glm::vec3(glm::rotate(-lForce/lNodes.at(i)->fVel/1000.f, lNodes.at(i)->vNorm) * glm::vec4(lNodes.at(i)->vLat, 0.f)));
        //lNodes.at(i+1)->vDir = glm::normalize(lNodes.at(i+1)->vDir);
        //lNodes.at(i+1)->vLat = glm::normalize(lNodes.at(i+1)->vLat);

        //qDebug("newDir: %f, %f, %f", lNodes.at(i+1)->vDir.x, lNodes.at(i+1)->vDir.y, lNodes.at(i+1)->vDir.z);
        lNodes.at(i+1)->vPos += lNodes.at(i+1)->vDir*(lNodes.at(i+1)->fVel/2000.f) + lNodes.at(i)->vDir*(lNodes.at(i+1)->fVel/2000.f);

        lNodes.at(i+1)->updateNorm();

        lNodes.at(i+1)->setRoll(CurveRoll->getEulerY(hDist+lNodes[i+1]->fVel/1000.f)* (180/M_PI)*(lNodes[i+1]->fVel/1000.f)); // - CurveRoll->getEulerY(i/1000.f));

        if(bOrientation == 1)
        {
            calcDirFromLast(i+1);
            lNodes[i+1]->setRoll(glm::dot(lNodes[i+1]->vDir, glm::vec3(0.f, -1.f, 0.f))*lNodes[i+1]->fYawFromLast);
        }

        lNodes.at(i+1)->updateNorm();

        lNodes.at(i+1)->fDistFromLast = glm::distance(lNodes.at(i+1)->fPosHeart(parent->fHeart), lNodes.at(i)->fPosHeart(parent->fHeart));
        lNodes.at(i+1)->fTotalLength += lNodes.at(i+1)->fDistFromLast;
        lNodes.at(i+1)->fHeartDistFromLast = 0.7f*glm::distance(lNodes.at(i+1)->vPos, lNodes.at(i)->vPos) + 0.3f*lNodes.at(i+1)->fDistFromLast;
        lNodes.at(i+1)->fTotalHeartLength += lNodes.at(i+1)->fHeartDistFromLast;
        lNodes.at(i+1)->fRollSpeed = CurveRoll->getEulerY(hDist+lNodes[i+1]->fVel/1000.f)* (180/M_PI) *lNodes[i+1]->fVel;  // /1000.f/lNodes.at(i+1)->fDistFromLast;
        if(bOrientation == 1)
        {
            lNodes.at(i+1)->fRollSpeed += glm::dot(lNodes[i+1]->vDir, glm::vec3(0.f, -1.f, 0.f))*lNodes[i+1]->fYawFromLast*1000.f;
        }


        calcDirFromLast(i+1);
        float temp = cos(fabs(lNodes[i+1]->getPitch())*M_PI/180.f);
        float forceAngle = sqrt(temp*temp*lNodes.at(i+1)->fYawFromLast*lNodes.at(i+1)->fYawFromLast + lNodes.at(i+1)->fPitchFromLast*lNodes.at(i+1)->fPitchFromLast);//deltaAngle;
        lNodes.at(i+1)->fAngleFromLast = forceAngle;

        if(bSpeed)
        {
            float heightDiff = lNodes.at(i+1)->fPosHeart(parent->fHeart).y - lNodes.at(i)->fPosHeart(parent->fHeart).y;
            lNodes.at(i+1)->fVel = glm::sqrt(lNodes.at(i)->fVel*lNodes.at(i)->fVel - 2*9.08665*heightDiff);
            lNodes.at(i+1)->fVel = glm::sqrt(lNodes.at(i+1)->fVel * lNodes.at(i+1)->fVel - 2*lNodes.at(i+1)->fDistFromLast*parent->fFriction); // glm::length(forceVec + glm::vec3(0, 1.f, 0))
            //qDebug("cur Speed = %f", lNodes.at(i+1)->fVel);
        }
        else
        {
            lNodes.at(i+1)->fVel = this->fVel;
        }

        if(fabs(lNodes.at(i+1)->fAngleFromLast) < std::numeric_limits<float>::epsilon())
        {
            forceVec = glm::vec3(0.f, 1.f, 0.f);
        }
        else
        {
            forceVec = glm::vec3(0.f, 1.f, 0.f) + (float)((lNodes.at(i+1)->fVel*lNodes.at(i+1)->fVel) / (9.80665 * lNodes.at(i+1)->fVel/lNodes.at(i+1)->fAngleFromLast * 0.18f/M_PI)) * glm::normalize(glm::vec3(glm::rotate(lNodes[i+1]->fDirFromLast, -lNodes.at(i+1)->vDir)*glm::vec4(-lNodes.at(i+1)->vNorm, 0.f)));
        }
        lNodes.at(i+1)->forceNormal = - glm::dot(forceVec, glm::normalize(lNodes.at(i+1)->vNorm));
        lNodes.at(i+1)->forceLateral = - glm::dot(forceVec, glm::normalize(lNodes.at(i+1)->vLat));

        this->length += lNodes.at(i+1)->fDistFromLast;
        hDist += lNodes[++i]->fHeartDistFromLast;
    }*/
}

void secforced::plotAdditionalGraphes(float x, mnode *lNode) {
    valuesRollX.append(x);
    valuesRollY.append((lNode->fRoll / (180/M_PI)) * 2);

    valuesRollDerivativesX.append(x);
    valuesRollDerivativesY.append(CurveRoll->getQuaternionDerivative(x));
}

void secforced::updateGraphStates() {
    CurveRoll->pointPlot->Mutex->lock();

    CurveRoll->pointPlot->plotter->graph(1)->setVisible(showRoll);
    CurveRoll->pointPlot->plotter->graph(1)->setData(valuesRollX, valuesRollY);

    CurveRoll->pointPlot->plotter->graph(2)->setVisible(showShowRollDerivative);
    CurveRoll->pointPlot->plotter->graph(2)->setData(valuesRollDerivativesX, valuesRollDerivativesY);

    CurveRoll->pointPlot->Mutex->unlock();

    trackBuilder->graphChanges = true;
}

double secforced::getMaxArgument()
{
    double minXValue = std::min(CurveRoll->points[CurveRoll->points.size()-1]->x, CurveVert->points[CurveVert->points.size()-1]->x);
    if(CurveLat->points.size() > 1) minXValue = std::min(minXValue, (double)CurveLat->points[CurveLat->points.size()-1]->x);

    return minXValue;
}
