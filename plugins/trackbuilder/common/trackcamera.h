#ifndef TRACKCAMERA_H
#define TRACKCAMERA_H

#include <common/global.h>

#include <QMouseEvent>
#include <QCursor>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "common/camerabase.h"

class QEvent;
class QTimer;
class GLWidget;

class TrackCamera : public CameraBase
{
    Q_OBJECT
public:
    TrackCamera(GLWidget &_glw);

    void moveForwards();
    void moveForwards(bool reverse, bool phantom = false);

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

    QMouseEvent *lastMouseEvent;

    glm::vec3 maxCamera;

    // Keyboard inputs
    QTimer *timerKeyPressed;
    QKeyEvent *lastKeyEvent;
    bool lockKey;
    int lastKey;

    int povPos;
public slots:
    void whileKeyPressed();
};


#endif // TRACKCAMERA_H
