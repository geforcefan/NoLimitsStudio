#include "section.h"
#include <cmath>
#include "glm/glm.hpp"
#include "track.h"

#define RELTHRESH 1.0f

using namespace std;

section::section()
{
}

section::section(track* getParent, enum secType _type, mnode* first)
{
    lNodes.append(first);
    parent = getParent;
    type = _type;
}

section::~section()
{
    while(lNodes.size() > 2)
    {
        delete lNodes.at(1);
        lNodes.removeAt(1);
    }
}

void section::fillPointList(QList<glm::vec4> &List, QList<glm::vec3> &Normals, mnode* anchor, float mPerNode, float fHeart)
{
    lNodes.at(0)->updateNorm();

    float fThreshold = 0.0;

    int numNodes = (int)(this->length / mPerNode);

    for(int i = 1; i < lNodes.size(); i++)
    {
        lNodes.at(i)->updateNorm();
        fThreshold += glm::distance(lNodes.at(i)->fPosHeart(fHeart), lNodes.at(i-1)->fPosHeart(fHeart));
        //qDebug("Node %d processed: Threshold is now %f", i, fThreshold);
        glm::vec3 V(anchor->vDir.x, 0, anchor->vDir.z), Norm;
        glm::vec4 P1;
        float temp = glm::length(V);
        glm::mat4 anchorBase;
        if(anchor->vDir.z != 0 || anchor->vDir.x != 0)
        {
            anchorBase = glm::mat4(glm::transpose(glm::mat4(-anchor->vDir.z/temp, 0.f, anchor->vDir.x/temp, 0.f,
                             0.f, 1.f, 0.f, 0.f,
                             -anchor->vDir.x/temp, 0.f, -anchor->vDir.z/temp, 0.f,
                             0.f, 0.f, 0.f, 1.f))
                             * glm::mat4(1.f, 0.f, 0.f, 0.f,
                             0.f, 1.f, 0.f, 0.f,
                             0.f, 0.f, 1.f, 0.f,
                             -anchor->fPosHeartx(fHeart), -anchor->fPosHearty(fHeart), -anchor->fPosHeartz(fHeart), 1.f));
        }
        else
        {
            anchorBase = glm::mat4(glm::transpose(glm::mat4(-anchor->vNorm.z/temp, 0.f, anchor->vNorm.x/temp, 0.f,
                             0.f, 1.f, 0.f, 0.f,
                             -anchor->vNorm.x/temp, 0.f, -anchor->vNorm.z/temp, 0.f,
                             0.f, 0.f, 0.f, 1.f))
                             * glm::mat4(1.f, 0.f, 0.f, 0.f,
                             0.f, 1.f, 0.f, 0.f,
                             0.f, 0.f, 1.f, 0.f,
                             -anchor->fPosHeartx(fHeart), -anchor->fPosHearty(fHeart), -anchor->fPosHeartz(fHeart), 1.f));
        }

        if(i == this->lNodes.size()-1 || fThreshold > this->length/numNodes)
        {
            P1 = anchorBase*(glm::vec4(glm::vec3(lNodes.at(i)->fPosHeart(fHeart)), 1.f));
            Norm = glm::vec3(anchorBase*(glm::vec4(lNodes[i]->vNorm, 0.f)));
            List.append(glm::vec4(glm::vec3(P1), lNodes.at(i)->fRoll*M_PI/180));
            Normals.append(Norm);
            fThreshold -= this->length/numNodes;
        }
    }
}

void section::iFillPointList(QList<int> &List, float mPerNode, float fHeart)
{
    lNodes.at(0)->updateNorm();

    float fThreshold = 0.0;

    int numNodes = (int)(this->length / mPerNode);
    int nodeCount = 0;

    for(int i = 1; i < lNodes.size(); i++)
    {
        lNodes.at(i)->updateNorm();
        fThreshold += lNodes[i]->fDistFromLast; //glm::distance(lNodes.at(i)->fPosHeart(fHeart), lNodes.at(i-1)->fPosHeart(fHeart));
        //qDebug("Node %d processed: Threshold is now %f", i, fThreshold);
        if(i == this->lNodes.size()-1 || fThreshold > this->length/numNodes)
        {
            List.append(parent->getNumPoints(this)+i-1);
            fThreshold -= this->length/numNodes;
            ++nodeCount;
        }
    }
    if(nodeCount > numNodes)
    {
        List.removeAt(List.size()-2);
    }
}

void section::calcDirFromLast(int i)
{
    if(i == 0 || i >= lNodes.size())
    {
        qDebug("wrong call of calcDirFromLast()");
        return;
    }
    glm::vec3 diff = lNodes[i]->vDir -lNodes[i-1]->vDir;
    if(diff.length() <= std::numeric_limits<float>::epsilon())
    {
        lNodes[i]->fDirFromLast = 0.f;
        lNodes[i]->fPitchFromLast = 0.f;
        lNodes[i]->fYawFromLast = 0.f;
    }
    double y = -glm::dot(diff, lNodes[i-1]->vNorm);
    double x = -glm::dot(diff, lNodes[i-1]->vLat);
    double angle = atan2(x, y)*180.f/M_PI;
    lNodes[i]->fDirFromLast = angle;
    lNodes[i]->fPitchFromLast = lNodes[i]->getPitch()-lNodes[i-1]->getPitch();
    lNodes[i]->fYawFromLast = lNodes[i]->getDirection()-lNodes[i-1]->getDirection();
    float fTrackPitchFromLast = 180.f/M_PI*(asin(lNodes[i]->vDirHeart(parent->fHeart).y) - asin(lNodes[i-1]->vDirHeart(parent->fHeart).y));
    float fTrackYawFromLast = 180.f/M_PI*(glm::atan2(-lNodes[i]->vDirHeart(parent->fHeart).x, -lNodes[i]->vDirHeart(parent->fHeart).z) - glm::atan2(-lNodes[i-1]->vDirHeart(parent->fHeart).x, -lNodes[i-1]->vDirHeart(parent->fHeart).z));
    float temp = cos(fabs(asin(lNodes[i]->vDirHeart(parent->fHeart).y)));
    lNodes[i]->fTrackAngleFromLast = sqrt(temp*temp*fTrackYawFromLast*fTrackYawFromLast + fTrackPitchFromLast * fTrackPitchFromLast);
    if(lNodes[i]->fYawFromLast > 270.f)
    {
        lNodes[i]->fYawFromLast -= 360.f;
    }
    else if(lNodes[i]->fYawFromLast < -270.f)
    {
        lNodes[i]->fYawFromLast += 360.f;
    }
    return;
}

float section::getSpeed()
{
    if(bSpeed)
    {
        return lNodes[lNodes.size()-1]->fVel;
    }
    else
    {
        return fVel;
    }
}
