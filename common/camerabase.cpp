#include "camerabase.h"


CameraBase::CameraBase(GLWidget &_glw) : glw(_glw) {
}

glm::mat4 CameraBase::getViewMatrix() {
    return ViewMatrix;
}

glm::mat4 CameraBase::getProjectionMatrix() {
    return ProjectionMatrix;
}

glm::vec3 CameraBase::getPosition() {
    return position;
}

void CameraBase::setDimensions(int _width, int _height) {
    this->width = _width;
    this->height = _height;

    this->calculate();
    glw.render();
}

void CameraBase::render() {
    glLoadMatrixf(glm::value_ptr(ViewMatrix));
}
