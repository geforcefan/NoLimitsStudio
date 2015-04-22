#ifndef SECTION_H
#define SECTION_H

#include <QList>
#include <QString>
#include "mnode.h"
#include <sstream>


#define EULER true
#define QUATERNION false

#define TIME false
#define DISTANCE true

class track;

typedef struct sBezier
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
} tBezier;

enum secType
{
    anchor,
    straight,
    curved,
    forced,
    geometric,
    bezier
};

class section
{
public:
    section();
    section(track* getParent, enum secType _type, mnode* first);
    ~section();
    float length;
    virtual void updateSection(int node = 0) = 0;
    virtual void fillPointList(QList<glm::vec4> &List, QList<glm::vec3> &Normals, mnode* anchor, float mPerNode, float fHeart);
    virtual void iFillPointList(QList<int> &List, float mPerNode, float fHeart);
    virtual double getMaxArgument() = 0;
    float getSpeed();
    void calcDirFromLast(int i);
    QList<mnode*> lNodes;
    track* parent;

    enum secType type;

    bool bSpeed;
    float fVel;

    bool bOrientation;
    bool bArgument;

    // Straight Section Parameters
    float fHLength;

    // Curved Section Parameters
    float fAngle;
    float fRadius;
    float fDirection;
    float fLeadIn;
    float fLeadOut;

    // Forced/Geometric Section Parameters
    int iTime;
    QString sName;

    // Bezier Section Parameters
    QList<tBezier*> bezList;
};

#endif // SECTION_H
