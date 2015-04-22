#ifndef NLBANKING_H
#define NLBANKING_H

#include <string>
#include <vector>
#include <map>

#include <common/global.h>


#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "plugins/trackbuilder/track/mnode.h"

typedef struct {
   glm::vec3 tangent;
   glm::vec3 normal;
   glm::vec3 binormal;
} bankingValues;


typedef struct bezier_s
{
    glm::vec3 Kp1;
    glm::vec3 Kp2;
    glm::vec3 P1;
    float roll;
    bool contRoll;
    bool equalDist;
    bool relRoll;

    float ptf;
    float fvdRoll;
    float length;
    int numNodes;
} bezier_t;

typedef std::map<float, int> TimelNodeIndex;

class NLBanking
{
public:
    NLBanking();

    void update();
    void storeBanking();
    float getBanking(int bezierIndex, float tfind);
    bankingValues getDir(int bezierIndex, float t);

    glm::vec3 lastNormal;

    std::vector<mnode*> lNodes;

    bool resetTimeValue;
private:
    std::map<int, TimelNodeIndex> lNodesBezierIndicies;

    // Bezier Section Parameters
    void calcDirFromLast(int i);
    std::vector<bezier_t*> bezList;
};

#endif // NLBANKING_H
