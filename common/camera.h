#ifndef CAMERA_H
#define CAMERA_H

#include <common/global.h>

#include <QMouseEvent>
#include <QCursor>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camerabase.h"

class QEvent;
class QTimer;
class GLWidget;

class Camera : public CameraBase
{
    Q_OBJECT
public:
    Camera(GLWidget &_glw);

    void moveForwards();
    void moveForwards(bool reverse);

    void calculate();
    bool eventFilterFake(QObject *obj, QEvent *event);

    glm::vec3 direction;
    bool useSpeedUp;

private:
    float speed;
    float mouseSpeed;

    float horizontalAngle;
    float verticalAngle;
    float initialFoV;

    // Inputs (mouse, keyboard, etc.)
    int lastButtonState;
    QPoint lastCursorPos;
    int cameraMode;

    QCursor *pCursor;
    QTimer *timerMousePressed;

    QMouseEvent *lastMouseEvent;

    glm::vec3 maxCamera;

    // Keyboard inputs
    QTimer *timerKeyPressed;
    QKeyEvent *lastKeyEvent;
    bool lockKey;
    int lastKey;

public slots:
    void whileMousePressed();
    void whileKeyPressed();
};

#endif // CAMERA_H
