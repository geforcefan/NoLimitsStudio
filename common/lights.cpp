#include <common/shader.h>
#include "lights.h"

lights::lights() {
}

void lights::addLight(glm::vec3 _position, glm::vec3 _lookat, glm::vec4 _colorDiffuse, glm::vec4 _colorAmbient, glm::vec4 _colorSpecular) {
    int numLights =  objLights.size();

    light tmp_light;
    tmp_light.position[0] = _position.x;
    tmp_light.position[1] = _position.y;
    tmp_light.position[2] = _position.z;
    tmp_light.position[3] = 1.0f;

    tmp_light.lookat[0] = _lookat.x;
    tmp_light.lookat[1] = _lookat.y;
    tmp_light.lookat[2] = _lookat.z;

    tmp_light.diffuse[0] = _colorDiffuse.x;
    tmp_light.diffuse[1] = _colorDiffuse.y;
    tmp_light.diffuse[2] = _colorDiffuse.z;
    tmp_light.diffuse[3] = _colorDiffuse.w;

    tmp_light.ambient[0] = _colorAmbient.x;
    tmp_light.ambient[1] = _colorAmbient.y;
    tmp_light.ambient[2] = _colorAmbient.z;
    tmp_light.ambient[3] = _colorAmbient.w;

    tmp_light.sepcular[0] = _colorSpecular.x;
    tmp_light.sepcular[1] = _colorSpecular.y;
    tmp_light.sepcular[2] = _colorSpecular.z;
    tmp_light.sepcular[3] = _colorSpecular.w;

    objLights.push_back(tmp_light);


}

light lights::getLight(int index) {
    return objLights[index];
}

void lights::setUniforms(int index) {
    light tmp_light = objLights[index];

    glUniform3fv(NoLimitsRenderer::currentShader->uniformLocation("lightPosition") , 1, glm::value_ptr(
                     glm::vec3(tmp_light.position[0], tmp_light.position[1], tmp_light.position[2])
                     ));

    glUniform3fv(NoLimitsRenderer::currentShader->uniformLocation("lightDir") , 1, glm::value_ptr(
                     glm::vec3(tmp_light.lookat[0], tmp_light.lookat[1], tmp_light.lookat[2])
                     ));

    glUniform4fv(NoLimitsRenderer::currentShader->uniformLocation("lightColor") , 1, glm::value_ptr(
                     glm::vec4(tmp_light.diffuse[0], tmp_light.diffuse[1], tmp_light.diffuse[2], tmp_light.diffuse[3])
                     ));

    glUniform4fv(NoLimitsRenderer::currentShader->uniformLocation("lightAmbient") , 1, glm::value_ptr(
                     glm::vec4(tmp_light.ambient[0], tmp_light.ambient[1], tmp_light.ambient[2], tmp_light.ambient[3])
                     ));
}
