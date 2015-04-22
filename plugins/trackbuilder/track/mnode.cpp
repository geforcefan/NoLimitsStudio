#include "mnode.h"
#include <cmath>


using namespace std;

mnode::mnode()
{

}

mnode::mnode(glm::vec3 getPos, glm::vec3 getDir, float getRoll, float getVel, float getNForce, float getLateral)
{
    this->vPos = getPos;
    this->vDir = getDir;
    this->vDir = glm::normalize(vDir);
    this->fRoll = getRoll;
    this->fVel = getVel;
    this->forceNormal = getNForce;
    this->forceLateral = getLateral;
    this->fDistFromLast = 0.0;
    this->fHeartDistFromLast = 0.0;
    this->fAngleFromLast = 0.0;
    this->fTrackAngleFromLast = 0.0;
    this->fDirFromLast = 0.0;
    this->fPitchFromLast = 0.0;
    this->fYawFromLast = 0.0;
    this->fTotalLength = 0.0;
    this->fTotalHeartLength = 0.0;
    this->fTime = 0.0f;

    if(this->vDir.y == 1)
    {
        this->vLat = glm::vec3(glm::rotate(getRoll, glm::vec3(0.f, -1.f, 0.f))*glm::vec4(1.f, 0.f, 0.f, 0.f));
    }
    else
    {
        this->vLat = glm::vec3(-this->vDir.z, 0, this->vDir.x);
    }

    this->vLat.y = tan(fRoll*M_PI/180)*sqrt(this->vLat.x*this->vLat.x+this->vLat.z*this->vLat.z);
    this->vLat = glm::normalize(vLat);
    this->fRollSpeed = 0.0;
}

void mnode::setRoll(float dRoll)
{
    this->vLat = glm::vec3(glm::rotate(-(dRoll), this->vDir)*glm::vec4(this->vLat, 0));

    this->updateRoll();

    return;
}

void mnode::updateRoll()
{
    this->updateNorm();
    glm::vec3 temp;
    if(vDir.y == 1)
    {
        temp = this->vLat;
    }
    else if(vDir.y != 0)
        temp = glm::vec3(glm::rotate((float)(glm::atan2(fabs(this->vDir.y), sqrt(this->vDir.x*this->vDir.x+this->vDir.z*this->vDir.z))*180/M_PI), glm::cross(vDir, glm::normalize(glm::vec3(vDir.x, 0, vDir.z))))*glm::vec4(this->vLat, 0));
    else
        temp = vLat;

    if(this->vNorm.y < 0)
        this->fRoll = glm::atan2(temp.y, sqrt(temp.x*temp.x+temp.z*temp.z))*180/M_PI;
    else
        this->fRoll = 180-glm::atan2(temp.y, sqrt(temp.x*temp.x+temp.z*temp.z))*180/M_PI <= 180 ? 180-glm::atan2(temp.y, sqrt(temp.x*temp.x+temp.z*temp.z))*180/M_PI : -180-glm::atan2(temp.y, sqrt(temp.x*temp.x+temp.z*temp.z))*180/M_PI;

    this->fRoll = atan2(vLat.y, -vNorm.y)*180.f/M_PI;
    return;
}

void mnode::updateNorm()
{
    this->vNorm = glm::normalize(glm::cross(this->vDir, this->vLat));
}

float mnode::fPosHeartx(float fHeart)
{
    return this->vPos.x+this->vNorm.x*fHeart;
}

float mnode::fPosHearty(float fHeart)
{
    return this->vPos.y+this->vNorm.y*fHeart;
}

float mnode::fPosHeartz(float fHeart)
{
    return this->vPos.z+this->vNorm.z*fHeart;
}

glm::vec3 mnode::fPosHeart(float fHeart)
{
    return glm::vec3(this->vPos.x+this->vNorm.x*fHeart, this->vPos.y+this->vNorm.y*fHeart, this->vPos.z+this->vNorm.z*fHeart);
}

float mnode::getPitch()
{
    return glm::atan2(vDir.y, glm::sqrt(vDir.x*vDir.x+vDir.z*vDir.z))*180/M_PI;
}

float mnode::getPitchChange()
{
    return fPitchFromLast*1000.f;
    float sign = fabs(fRoll) > 90 ? -1 : 1;
    return sign*cos((this->fDirFromLast+fRoll) * M_PI/180.f)*this->fAngleFromLast*1000.f;
}

float mnode::getDirection()
{
    return glm::atan2(-vDir.x, -vDir.z)*180/M_PI;
}

float mnode::getYawChange()
{
    return fYawFromLast*1000.f;
}

float mnode::fFlexion()
{
    return fDistFromLast <= 0.0 ? 0.0f : fTrackAngleFromLast / fDistFromLast;   // needs overhaul
}

void mnode::changePitch(float dAngle, bool inverted)
{
    glm::vec3 rotateAround;
    if(inverted == true)
    {
        if(fabs(vLat.y) < 1.9f) //sqrt(vLat.x*vLat.x + vLat.z*vLat.z))
        {
            rotateAround = -glm::normalize(glm::cross(glm::vec3(0, vNorm.y, 0), vDir));
        }
        else
        {
            rotateAround = (2.f*(fRoll < 0)-1.f) * glm::normalize(glm::cross(glm::vec3(0, vLat.y, 0), vDir));
        }
    }
    else
    {
        if(fabs(vLat.y) < 1.9f) //sqrt(vLat.x*vLat.x + vLat.z*vLat.z))
        {
            rotateAround = glm::normalize(glm::cross(glm::vec3(0, vNorm.y, 0), vDir)); //(2.f*(fabs(fRoll) < 90.f)-1.f) *
        }
        else
        {
            rotateAround = (1.f-2.f*(fRoll < 0)) * glm::normalize(glm::cross(glm::vec3(0, -vLat.y, 0), vDir));
        }
    }

    vDir = glm::normalize(glm::vec3(glm::rotate(dAngle, rotateAround) * glm::vec4(vDir, 0.f)));
    vLat = glm::normalize(glm::vec3(glm::rotate(dAngle, rotateAround) * glm::vec4(vLat, 0.f)));
    this->updateNorm();
}

void mnode::changeYaw(float dAngle)
{
    vDir = glm::normalize(glm::vec3(glm::rotate(dAngle, glm::vec3(0.f, 1.f, 0.f))*glm::vec4(vDir, 0.f)));
    vLat = glm::normalize(glm::vec3(glm::rotate(dAngle, glm::vec3(0.f, 1.f, 0.f))*glm::vec4(vLat, 0.f)));
    this->updateNorm();
}

glm::vec3 mnode::vLatHeart(float fHeart)
{
    float estimated;
    if(fAngleFromLast < 0.001f)
    {
        estimated = fHeartDistFromLast;
    }
    else
    {
        estimated = cos(fDirFromLast*M_PI/180.f)*cos(fDirFromLast*M_PI/180.f)*fDistFromLast + sin(fDirFromLast*M_PI/180.f)*sin(fDirFromLast*M_PI/180.f)*fHeartDistFromLast;
    }

    float fRollSpeedPerMeter = fHeartDistFromLast > 0.f ? fRollSpeed/1000.f/estimated : 0.f;
    return glm::normalize(glm::normalize(vLat) - glm::normalize(vDir)*(float)(fRollSpeedPerMeter*M_PI*fHeart/180.f));
}

glm::vec3 mnode::vDirHeart(float fHeart)
{
    float estimated;
    if(fAngleFromLast < 0.001f)
    {
        estimated = fHeartDistFromLast;
    }
    else
    {
        estimated= cos(fDirFromLast*M_PI/180.f)*cos(fDirFromLast*M_PI/180.f)*fDistFromLast + sin(fDirFromLast*M_PI/180.f)*sin(fDirFromLast*M_PI/180.f)*fHeartDistFromLast;
    }
    float fRollSpeedPerMeter = fHeartDistFromLast > 0.f ? fRollSpeed/1000.f/estimated : 0.f;
    if(fRollSpeedPerMeter != fRollSpeedPerMeter)
        fRollSpeedPerMeter = 0.f;
    return glm::normalize(glm::normalize(vDir) + glm::normalize(vLat)*(float)(fRollSpeedPerMeter*M_PI*fHeart/180.f));
}
