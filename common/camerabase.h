#ifndef CAMERABASE_H
#define CAMERABASE_H

#include <common/global.h>
#include <maingl/glwidget.h>
#include <maingl/glrenderer.h>
#include <maingl/glthread.h>

#include <QObject>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class GLWidget;

class CameraBase : public QObject
{
    Q_OBJECT
public:
    CameraBase(GLWidget &_glw);

    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();
    glm::vec3 getPosition();
    void setDimensions(int _width, int _height);
    void render();

    virtual bool eventFilterFake(QObject *obj, QEvent *event) {}
    virtual void calculate() {}

    glm::vec3 position;
    glm::mat4 ViewMatrix;
    glm::mat4 ProjectionMatrix;

    int width;
    int height;

    GLWidget &glw;
};

#endif // CAMERABASE_H
