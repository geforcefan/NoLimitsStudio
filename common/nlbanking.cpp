#include "nlbanking.h"
#include <common/global.h>
#include <common/nlsegment.h>

#include <QDebug>
#include <cmath>

NLBanking::NLBanking()
{
    resetTimeValue = true;
}

void NLBanking::update() {
    storeBanking();
}


bankingValues NLBanking::getDir(int bezierIndex, float t) {
    bankingValues ret;
    ret.tangent = glm::vec3(1.0f);
    ret.normal = glm::vec3(0.0f, 1.0f, 0.0f);
    ret.binormal = glm::vec3(0.0f, 0.0f, 1.0f);

    if(bezierIndex > bezList.size()-1 || !lNodes.size()) return ret;

    TimelNodeIndex *tni = &lNodesBezierIndicies[bezierIndex];

    mnode *currNode = lNodes[lNodes.size()-1];

    bool found = false;
    float currt = 0.0f;
    int index = 0;

    std::map<float,int>::iterator it;
    for ( it=tni->begin() ; it != tni->end(); it++ ) {
        currt = (*it).first;
        index = (*it).second;

        if(t < currt) {
            found = true;
            break;
        }

        currNode = lNodes[index];
    }

    if(!found && t > 0.5) currNode = lNodes[tni->rbegin()->second];
    if(!found && t < 0.5) currNode = lNodes[tni->begin()->second];

    ret.tangent = glm::normalize(currNode->vDir);
    ret.normal = glm::normalize(currNode->vNorm);
    ret.binormal = -glm::normalize(currNode->vLat);

    return ret;
}

void NLBanking::storeBanking() {
    bezList.clear();

    if(!NoLimitsRenderer::Track.numBeziers) return;

    for(int i=0; i < NoLimitsRenderer::Track.numBeziers; i++) {
        bezier_t *bzr = new bezier_t();
        bzr->Kp1 = glm::vec3(NoLimitsRenderer::Track.beziers[i].cp1X, NoLimitsRenderer::Track.beziers[i].cp1Y, NoLimitsRenderer::Track.beziers[i].cp1Z);
        bzr->Kp2 = glm::vec3(NoLimitsRenderer::Track.beziers[i].cp2X, NoLimitsRenderer::Track.beziers[i].cp2Y, NoLimitsRenderer::Track.beziers[i].cp2Z);
        bzr->P1 = glm::vec3(NoLimitsRenderer::Track.beziers[i].posX, NoLimitsRenderer::Track.beziers[i].posY, NoLimitsRenderer::Track.beziers[i].posZ);
        bzr->contRoll = NoLimitsRenderer::Track.beziers[i].continuesRoll;
        bzr->relRoll = NoLimitsRenderer::Track.beziers[i].relativeRoll;
        bzr->equalDist = NoLimitsRenderer::Track.beziers[i].equalDistanceCP;
        bzr->ptf = 0.0f;
        bzr->fvdRoll = 0.0f;
        bzr->roll = NoLimitsRenderer::Track.beziers[i].roll;

        bezList.push_back(bzr);
    }

    if(NoLimitsRenderer::Track.trackClosedCircuit) {
        bezList.push_back(bezList[0]);
        bezList.push_back(bezList[1]);
    }

    QList<float> tList;

    lNodesBezierIndicies.clear();
    lNodes.clear();
    lNodes.push_back(new mnode(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0, 0, -1), 0.0f, 10.f, 1.0f, 0.0f));
    lNodes.at(0)->updateNorm();

    int cur = 0, lastcur = 0;
    float t = 0.f;

    for(int b = 0; b < bezList.size()-1; b++) {
        while(t <= 1.f) {
            tList.append(t);

            int bnext = (b+1)%bezList.size();
            float t1 = 1.f-t;
            if(cur >= lNodes.size()) {
                lNodes.push_back(new mnode(*lNodes.back()));
            }

            lNodesBezierIndicies[b][t] = cur;

            lNodes[cur]->vPos = t1*t1*t1*bezList[b]->P1 + 3.f*t1*t1*t*bezList[b]->Kp2 + 3.f*t1*t*t*bezList[bnext]->Kp1 + t*t*t*bezList[bnext]->P1;

            lNodes[cur]->fRoll = t1*bezList[b]->fvdRoll + t*bezList[bnext]->fvdRoll;
            lNodes[cur]->fRoll *= 180.f/M_PI;

            glm::vec3 diff1 = bezList[b]->Kp2-bezList[b]->P1;
            glm::vec3 diff2 = bezList[bnext]->Kp1-bezList[b]->Kp2;
            glm::vec3 diff3 = bezList[bnext]->P1-bezList[bnext]->Kp1;

            lNodes[cur]->vDir = t1*t1*diff1 + 2*t1*t*diff2 + t*t*diff3;

            float lengthDir = glm::length(lNodes[cur]->vDir);

            lNodes[cur]->vDir = glm::normalize(lNodes[cur]->vDir);

            lNodes[cur]->vLat.x = -lNodes[cur]->vDir.z;
            lNodes[cur]->vLat.y = 0;
            lNodes[cur]->vLat.z = lNodes[cur]->vDir.x;

            if(glm::length(lNodes[cur]->vLat) < std::numeric_limits<float>::epsilon()) {
                lNodes[cur]->vLat = glm::normalize(glm::cross(lNodes[cur]->vNorm, lNodes[cur]->vDir));
            }

            lNodes[cur]->setRoll(0.f);

            if(cur) {
                lNodes[cur]->fHeartDistFromLast = glm::distance(lNodes[cur]->vPos, lNodes[cur-1]->vPos);
                lNodes[cur]->fTotalHeartLength += lNodes[cur]->fHeartDistFromLast;
                lNodes[cur]->fVel = lNodes[cur]->fHeartDistFromLast*1000;
            }

            t += 1.f/(3000.f*lengthDir/10);

            ++cur;
        }
        t = (resetTimeValue?0.f:t-1.0f);

        bezList[b]->length = lNodes[cur-1]->fTotalHeartLength - lNodes[lastcur]->fTotalHeartLength;
        bezList[b]->numNodes = cur-1-lastcur;
        lastcur = cur-1;
    }

    if(!tList.size()) return;
    if(!lNodes.size()) return;


    int b = 0;
    float correction = 0;

    bezList[0]->fvdRoll = bezList[0]->roll;

    for(int i = 0; i < lNodes.size(); ++i) {
        calcDirFromLast(i); // now get roll change you need to get from a to b
        if(i && tList[i] < tList[i-1]) {
            ++b;
            if(bezList[b]->relRoll) {
                bezList[b]->fvdRoll = bezList[b-1]->fvdRoll + correction*M_PI/180.f + bezList[b]->roll;
                bezList[b-1]->ptf = bezList[b]->roll;
            } else {
                bezList[b]->fvdRoll = bezList[b]->roll;
                bezList[b-1]->ptf = bezList[b]->fvdRoll - bezList[b-1]->fvdRoll - correction*M_PI/180.f;
                if(fabs(bezList[b-1]->ptf) > M_PI) bezList[b-1]->ptf += bezList[b-1]->ptf > 0.f ? -2.f*M_PI : 2.f*M_PI;
            }
            correction = 0.f;
        }
        correction -= glm::dot(lNodes[i]->vDir, glm::vec3(0.f, -1.f, 0.f))*lNodes[i]->fYawFromLast;

        float fRoll = bezList[b]->fvdRoll*180.f/M_PI;

        lNodes[i]->setRoll(fRoll + correction);
    }

    b = 0;
    int bNext = 1;
    float startVal, endVal, area, a1, b1, c1;
    float value = 0.f;

    for(int i = 0; i < lNodes.size(); ++i) {
        float tNext;
        if(i == lNodes.size()-1 || tList[i+1] < tList[i]) tNext = 1.f;
        else tNext = tList[i+1];

        if(i && tList[i] < tList[i-1]) {
            ++b;
            bNext = (b+1)%bezList.size();
            value = 0.f;

            startVal = endVal;

            if(bezList[bNext]->contRoll) endVal = (bezList[b]->length*bezList[b]->ptf + bezList[bNext]->length*bezList[bNext]->ptf)/(bezList[b]->length + bezList[bNext]->length);//*(tNext - tList[i]);
            else endVal = 0.f;

            area = bezList[b]->ptf;

            a1 = 3.f*startVal + 3.f*endVal - 6.f*area;
            b1 = 6.f*area - 4.f*startVal - 2.f*endVal;
            c1 = startVal;
        } else if(!i) {
            startVal = 0.f;
            value = 0.f;
            if(bezList.size() > 1 && bezList[1]->contRoll) endVal = (bezList[b]->length*bezList[b]->ptf + bezList[bNext]->length*bezList[bNext]->ptf)/(bezList[b]->length + bezList[bNext]->length);
            else endVal = 0.f;

            area = bezList[0]->ptf;

            a1 = 3.f*startVal + 3.f*endVal - 6.f*area;
            b1 = 6.f*area - 4.f*startVal - 2.f*endVal;
            c1 = startVal;
        }
        value += (c1 + tList[i]*(b1+ a1*tList[i]))*180.f/M_PI * (tNext - tList[i]);

        lNodes[i]->setRoll(value);

        if(i) {
            lNodes[i]->fDistFromLast = glm::distance(lNodes[i]->fPosHeart(0.01f), lNodes[i-1]->fPosHeart(0.01f));
            lNodes[i]->fTotalLength = lNodes[i-1]->fTotalLength + lNodes[i]->fDistFromLast;
            lNodes[i]->fRollSpeed = (c1 + tList[i]*(b1+ a1*tList[i]))*1000*180.f/M_PI * (tNext - tList[i]);//(lNodes[i]->fRoll - lNodes[i-1]->fRoll - glm::dot(lNodes[i]->vDir, glm::vec3(0.f, -1.f, 0.f))*lNodes[i]->fYawFromLast)*1000.f;
        }
    }

    if(NoLimitsRenderer::Track.trackClosedCircuit) {
        lNodes.resize(lNodes.size() - bezList[bezList.size() - 2]->numNodes);
    }
}

void NLBanking::calcDirFromLast(int i) {
    if(i == 0 || i >= lNodes.size()) return;

    glm::vec3 diff = lNodes[i]->vDir - lNodes[i-1]->vDir;

    if(diff.length() <= std::numeric_limits<float>::epsilon()) {
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

    float fTrackPitchFromLast = 180.f/M_PI*(asin(lNodes[i]->vDirHeart(0.01f).y) - asin(lNodes[i-1]->vDirHeart(0.01f).y));
    float fTrackYawFromLast = 180.f/M_PI*(glm::atan2(-lNodes[i]->vDirHeart(0.01f).x, -lNodes[i]->vDirHeart(0.01f).z) - glm::atan2(-lNodes[i-1]->vDirHeart(0.01f).x, -lNodes[i-1]->vDirHeart(0.01f).z));
    float temp = cos(fabs(asin(lNodes[i]->vDirHeart(0.01f).y)));

    lNodes[i]->fTrackAngleFromLast = sqrt(temp*temp*fTrackYawFromLast*fTrackYawFromLast + fTrackPitchFromLast * fTrackPitchFromLast);

    if(lNodes[i]->fYawFromLast > 270.f) lNodes[i]->fYawFromLast -= 360.f;
    else if(lNodes[i]->fYawFromLast < -270.f) lNodes[i]->fYawFromLast += 360.f;

    return;
}

float NLBanking::getBanking(int bezierIndex, float tfind) {
}
