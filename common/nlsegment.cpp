#include <common/global.h>
#include <common/nlbanking.h>
#include "NLSegment.h"

NLSegment::~NLSegment() {
    clearOffsetTable();
}

void NLSegment::clearOffsetTable() {
    for(int i=0; i < offsetTable.size(); i++) delete offsetTable[i];
    offsetTable.clear();
    getLengthNextPlot = 0.0f;
}

NLSegment::NLSegment(int _segmentIndex) : segmentIndex(_segmentIndex)
{
    segment = NoLimitsRenderer::Track.segs;

    int firstSegmentIndex;
    int secondSegmentIndex;

    firstSegmentIndex = this->segmentIndex;
    secondSegmentIndex = (NoLimitsRenderer::Track.totalSegments-1 == this->segmentIndex && NoLimitsRenderer::Track.trackClosedCircuit)?0:this->segmentIndex + 1;

    P0 = glm::vec3(NoLimitsRenderer::Track.beziers[ firstSegmentIndex ].posX, NoLimitsRenderer::Track.beziers[ firstSegmentIndex ].posY, NoLimitsRenderer::Track.beziers[ firstSegmentIndex ].posZ);
    P1 = glm::vec3(NoLimitsRenderer::Track.beziers[ firstSegmentIndex ].cp2X, NoLimitsRenderer::Track.beziers[ firstSegmentIndex ].cp2Y, NoLimitsRenderer::Track.beziers[ firstSegmentIndex ].cp2Z);
    P2 = glm::vec3(NoLimitsRenderer::Track.beziers[ secondSegmentIndex ].cp1X, NoLimitsRenderer::Track.beziers[ secondSegmentIndex ].cp1Y, NoLimitsRenderer::Track.beziers[ secondSegmentIndex ].cp1Z);
    P3 = glm::vec3(NoLimitsRenderer::Track.beziers[ secondSegmentIndex ].posX, NoLimitsRenderer::Track.beziers[ secondSegmentIndex ].posY, NoLimitsRenderer::Track.beziers[ secondSegmentIndex ].posZ);

    totalLength = 0.00f;

    // Get Length inital stuff
    getLengthNewVec = P0 - 3.0f*(P1 - P2) - P3;

    getLengthA = 9 * glm::dot(getLengthNewVec,getLengthNewVec);
    getLengthB = 36 * glm::dot(2.0f*P1 - P0 - P2, getLengthNewVec);
    getLengthC = 18 * (glm::dot(3.0f*P0,P0) + glm::dot(11.0f*P1,P1) + glm::dot(2.0f*P2,P2) + glm::dot(P1,P3 - 11.0f*P2) - glm::dot(P0,12.0f*P1 - 7.0f*P2 + P3));
    getLengthD = 36 * glm::dot(P1 - P0, P0 - 2.0f*P1 + P2);
    getLengthE = 9 * glm::dot(P1 - P0,P1 - P0);

    // For Offset
    getLengthNextPlot = 0.0f;
}

#include <QDebug>

float NLSegment::getLength() { return getLength(false); }
float NLSegment::getLength(bool generateOffsetTable) {
    if(((totalLength == 0.00f || !totalLength || totalLength!=totalLength)) || generateOffsetTable) {
        clearOffsetTable();
        float _totalLength = 0.0f;
        float t = 0.0f;

        float fT = sqrt(getLengthE);
        float fmT = getLengthD/(2*fT);

        while(t<1) {
            if(generateOffsetTable && getLengthNextPlot - _totalLength <= 0) {
                getLengthNextPlot++;
                offsetTable.push_back(new NoLimitsStudio::OffsetPoint(t,_totalLength));
            }
            float fmmT = (getLengthC + 3.0f*t*(getLengthB + 2.0f*getLengthA*t) - fmT*fmT)/fT;
            float tNext = std::min(1.0f,float(t + 1.0f/(10.0f + 400.0f*sqrt(fabs(fmmT)))));
            float fN = sqrt(getLengthE + tNext*(getLengthD + tNext*(getLengthC + tNext*(getLengthB + getLengthA*tNext))));
            float fmN = (getLengthD + tNext*(2.0f*getLengthC + tNext*(3.0f*getLengthB + 4.0f*getLengthA*tNext)))/(2.0f*fT);
            _totalLength += 1.0f/12.0f * (tNext - t) * (6.0f * (fT + fN) + (tNext - t)*(fmN - fmT));
            fT = fN;
            fmT = fmN;
            t = tNext;
        }

        if(generateOffsetTable) offsetTable.push_back(new NoLimitsStudio::OffsetPoint(t,_totalLength));
        totalLength = _totalLength;
    }

    return totalLength;
}

glm::vec3 NLSegment::getOffsetPosition(float offset) {
    if(!offsetTable.size()) getLength(true);
    if(offset > offsetTable[offsetTable.size()-1]->l) {
        lastOffset = 1.0f;
        return getRelativePosition(1.0f);
    }

    int startPoint = 0;

    while(offsetTable[startPoint+1]->l < offset) startPoint++;

    float t = offsetTable[startPoint]->x;
    float currentLength = offsetTable[startPoint]->l;

    float fT = sqrt(getLengthE + t*(getLengthD + t*(getLengthC + t*(getLengthB + getLengthA*t))));
    float fmT = (getLengthD + t*(2*getLengthC + t*(3*getLengthB + 4*getLengthA*t)))/(2*fT);
    float newT = 0.0f;

    while(true) {
        float fmmT = (getLengthC + 3.0f*t*(getLengthB + 2.0f*getLengthA*t) - fmT*fmT)/fT;
        float tNext = std::min(1.0f,float(t + 1.0f/(10.0f + 400.0f*sqrt(fabs(fmmT)))));
        float fN = sqrt(getLengthE + tNext*(getLengthD + tNext*(getLengthC + tNext*(getLengthB + getLengthA*tNext))));
        float fmN = (getLengthD + tNext*(2.0f*getLengthC + tNext*(3.0f*getLengthB + 4.0f*getLengthA*tNext)))/(2.0f*fT);
        float prevL = currentLength;
        currentLength += 1.0f/12.0f * (tNext - t) * (6.0f * (fT + fN) + (tNext - t)*(fmN - fmT));
        if(currentLength >= offset) {
            newT = (currentLength * t - offset * t + offset * tNext - prevL * tNext)/(currentLength - prevL);
            break;
        }

        fT = fN;
        fmT = fmN;
        t = tNext;
    }

    lastOffset = newT;
    return getRelativePosition(newT);
}

float NLSegment::getTimeFromOffset(float offset) {
    float time = 0.0f;
    if(!offsetTable.size()) getLength(true);

    offset = totalLength * offset;

    if(offset > offsetTable[offsetTable.size()-1]->l) {
        time = 1.0f;
        return time;
    }

    int startPoint = 0;

    while(offsetTable[startPoint+1]->l < offset) startPoint++;

    float t = offsetTable[startPoint]->x;
    float currentLength = offsetTable[startPoint]->l;

    float fT = sqrt(getLengthE + t*(getLengthD + t*(getLengthC + t*(getLengthB + getLengthA*t))));
    float fmT = (getLengthD + t*(2*getLengthC + t*(3*getLengthB + 4*getLengthA*t)))/(2*fT);
    float newT = 0.0f;

    while(true) {
        float fmmT = (getLengthC + 3.0f*t*(getLengthB + 2.0f*getLengthA*t) - fmT*fmT)/fT;
        float tNext = std::min(1.0f,float(t + 1.0f/(10.0f + 400.0f*sqrt(fabs(fmmT)))));
        float fN = sqrt(getLengthE + tNext*(getLengthD + tNext*(getLengthC + tNext*(getLengthB + getLengthA*tNext))));
        float fmN = (getLengthD + tNext*(2.0f*getLengthC + tNext*(3.0f*getLengthB + 4.0f*getLengthA*tNext)))/(2.0f*fT);
        float prevL = currentLength;
        currentLength += 1.0f/12.0f * (tNext - t) * (6.0f * (fT + fN) + (tNext - t)*(fmN - fmT));
        if(currentLength >= offset) {
            newT = (currentLength * t - offset * t + offset * tNext - prevL * tNext)/(currentLength - prevL);
            break;
        }

        fT = fN;
        fmT = fmN;
        t = tNext;
    }

    time = newT;
    return time;
}

glm::vec3 NLSegment::getRelativePosition(float t) {
    if(t > 1.0f) t = 1.0f;

    float t1 = 1.0f-t;
    float b0 = t1 * t1 * t1;
    float b1 = 3 * t1 * t1 * t;
    float b2 = 3 * t1 * t * t;
    float b3 = t * t * t;

    return glm::vec3(
                b0 * P0.x  + b1 * P1.x + b2 * P2.x + b3 * P3.x,
                b0 * P0.y  + b1 * P1.y + b2 * P2.y + b3 * P3.y,
                b0 * P0.z  + b1 * P1.z + b2 * P2.z + b3 * P3.z
                );

}

float NLSegment::getBanking(float t) {
    float b = NoLimitsRenderer::TrackBanking->getBanking(segmentIndex, t);//[3].x;
    return b;
}


bankingValues NLSegment::getDir(float t) {
    bankingValues b = NoLimitsRenderer::TrackBanking->getDir(segmentIndex, t);//[3].x;
    return b;
}
