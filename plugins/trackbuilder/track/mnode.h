#ifndef MNODE_H
#define MNODE_H

#include <QList>
#include <sstream>
#include <fstream>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

class mnode
{
public:
    mnode();
    mnode(glm::vec3 getPos, glm::vec3 getDir, float getRoll, float getVel, float getNForce, float getLateral);
    void setRoll(float dRoll);
    void updateRoll();
    void updateNorm();
    void changePitch(float dAngle, bool inverted);
    void changeYaw(float dAngle);
    float getPitchChange();
    float getYawChange();
    float fPosHeartx(float fHeart);
    float fPosHearty(float fHeart);
    float fPosHeartz(float fHeart);
    glm::vec3 vLatHeart(float fHeart);
    glm::vec3 vDirHeart(float fHeart);
    glm::vec3 fPosHeart(float fHeart);

    float getPitch();
    float getDirection();

    glm::vec3 vPos;
    glm::vec3 vDir;
    glm::vec3 vLat;
    glm::vec3 vNorm;
    float fRoll;
    float fVel;
    float forceNormal;
    float forceLateral;
    float fDistFromLast;
    float fHeartDistFromLast;
    float fAngleFromLast;
    float fTrackAngleFromLast;
    float fDirFromLast;
    float fPitchFromLast;
    float fYawFromLast;
    float fRollSpeed;
    float fFlexion();
    float fTotalLength;
    float fTotalHeartLength;
    float fTime;
};

#endif // MNODE_H
