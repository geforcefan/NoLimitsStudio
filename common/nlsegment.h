#ifndef NLSEGMENT_H
#define NLSEGMENT_H

#include <common/global.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <common/nlbanking.h>

namespace NoLimitsStudio {
    class OffsetPoint {
    public:
        OffsetPoint(float _x, float _l) { x = _x; l = _l;  }
        float x, l;
    };
}

class NLSegment
{
private:
    float totalLength;
public:
    int segmentIndex;
    glm::vec3 P0, P1, P2, P3;

    NLSegment(int segmentIndex);
    ~NLSegment();
    void clearOffsetTable();

    glm::vec3 getOffsetPosition(float offset);
    float getTimeFromOffset(float offset);
    glm::vec3 getRelativePosition(float t);
    float getBanking(float t);
    bankingValues getDir(float t);

    SEGMSegment *segment;

    float lastOffset;

    // offset table
    std::vector<NoLimitsStudio::OffsetPoint*> offsetTable;

    // getLength
    float getLength(bool generateOffsetTable);
    float getLength();
    glm::vec3 getLengthNewVec;
    float getLengthA, getLengthB, getLengthC, getLengthD, getLengthE;
    float getLengthNextPlot;
};

#endif // NLSEGMENT_H
