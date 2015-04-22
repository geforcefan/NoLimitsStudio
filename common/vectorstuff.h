#ifndef VECTORSTUFF_H
#define VECTORSTUFF_H

#include <common/global.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <map>

class vectorStuff
{
public:
    vectorStuff();
    glm::mat4x4 alignToVector(glm::vec3 point, glm::vec3 point2, float banking);
    glm::mat4x4 alignToVector(glm::vec3 point, glm::vec3 point2, float banking, glm::vec3 position, bool useLastPoint, glm::vec3 lastPoint);


    glm::vec3 bankingVector;
    glm::vec3 rotationVector;

    float angle(glm::vec3 cur, glm::vec3 vec);
};

#endif // VECTORSTUFF_H
