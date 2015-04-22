#ifndef LIGHTS_H
#define LIGHTS_H

#include <common/global.h>

typedef struct {
    float ambient[4];
    float diffuse[4];
    float sepcular[4];
    float position[4];
    float lookat[3];
} light;

class lights
{
public:
    lights();
    void addLight(glm::vec3 _position, glm::vec3 _lookat, glm::vec4 _colorDiffuse, glm::vec4 _colorAmbient, glm::vec4 _colorSpecular);
    light getLight(int index);
    void setUniforms(int index);
private:
    std::vector <light> objLights;
};

#endif // LIGHTS_H
