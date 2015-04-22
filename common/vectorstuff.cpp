#include <common/global.h>
#include "vectorstuff.h"

vectorStuff::vectorStuff()
{
    bankingVector = glm::vec3(0, 0, 1);
    rotationVector = glm::vec3(1, 0, 0);
}
glm::mat4x4 vectorStuff::alignToVector(glm::vec3 point, glm::vec3 point2, float banking) {
    return alignToVector(point, point2, banking, point, false, glm::vec3(0.0f));
}

glm::mat4x4 vectorStuff::alignToVector(glm::vec3 point, glm::vec3 point2, float banking, glm::vec3 position, bool useLastPoint, glm::vec3 lastPoint) {
    glm::vec3 normal;
    if(!useLastPoint) normal = point2 - point;
    else normal = ((point2 - point) + (point - lastPoint)) / 2.0f;
    normal = glm::normalize(normal);

    // Up Vector
    glm::vec3 upVector = glm::vec3(0, 1, 0);
    glm::vec3 upAxis = glm::cross(upVector, normal);
    upAxis = glm::normalize(upAxis);
    float upAngle = this->angle(upVector, normal);

    // Side Vector
    glm::vec3 sideVector = glm::vec3(0, 0, 1);
    glm::vec3 sideAxis = glm::cross(sideVector, upAxis);
    glm::vec3 sideAxisTmp = sideAxis;
    sideAxis = glm::normalize(sideAxis);

    float len = sqrt(sideAxisTmp.x * sideAxisTmp.x + sideAxisTmp.y * sideAxisTmp.y);
    if(len == 0.0f) {
        sideAxis = glm::vec3(0.0f,-1.0f,0.0f);
    }

    float sideAngle = this->angle(sideVector, upAxis);

    glm::mat4x4 objMatrix = glm::mat4x4();

    objMatrix = glm::translate(objMatrix, position);
    objMatrix = glm::rotate(objMatrix, upAngle, upAxis);
    objMatrix = glm::rotate(objMatrix, sideAngle, sideAxis);
    objMatrix = glm::rotate(objMatrix, 90.0f, this->rotationVector);
    objMatrix = glm::rotate(objMatrix, banking, this->bankingVector);

    return objMatrix;
}

float vectorStuff::angle( glm::vec3 cur, glm::vec3 vec ) {
    glm::vec3 n1 = glm::vec3(cur.x, cur.y, cur.z);
    n1 = glm::normalize(n1);
    glm::vec3 n2 = glm::vec3(vec.x, vec.y, vec.z);
    n2 = glm::normalize(n2);
    return (float)(acos( glm::dot(n1, n2) )/0.01745329252);
}
