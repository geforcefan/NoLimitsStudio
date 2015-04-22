#include <common/global.h>

#include <maingl/glwidget.h>
#include <QEvent>
#include <QKeyEvent>
#include <QTimer>
#include <QObject>

#include <QDebug>
#include <QApplication>

#include "camerabase.h"
#include "camera.h"

Camera::Camera(GLWidget &_glw): CameraBase(_glw) {
    speed = 0.15f; // 3 units / second

#ifdef Q_OS_WIN
    mouseSpeed = 0.004f;
#endif
#ifdef Q_OS_MAC
    mouseSpeed = 0.007f;
#endif

    // Initial position : on +Z
    position = glm::vec3( 0, 1, 0 );
    // Initial horizontal angle : toward -Z
    horizontalAngle = 3.14f;
    // Initial vertical angle : none
    verticalAngle = 0.0f;
    // Initial Field of View
    initialFoV = 45.0f;

    maxCamera = glm::vec3(220.0f, 150.0f, 220.0f);

    pCursor = new QCursor();
    cameraMode = false;
    useSpeedUp = false;

    // Timer for whileMousePressed
    timerMousePressed = new QTimer();
    timerMousePressed->setInterval(5);
    QObject::connect(this->timerMousePressed, SIGNAL(timeout()), this, SLOT(whileMousePressed()));

    // Timer for whileMousePressed
    timerKeyPressed = new QTimer();
    timerKeyPressed->setInterval(5);
    QObject::connect(this->timerKeyPressed, SIGNAL(timeout()), this, SLOT(whileKeyPressed()));
    lockKey = false;
}

void Camera::moveForwards()
{
    this->moveForwards(false);
}

void Camera::moveForwards(bool reverse)
{
    if(!reverse) position += direction * speed;
    else position -= direction * speed;
}

bool Camera::eventFilterFake(QObject *obj, QEvent *event) {
    //if(event->type() != 5) qDebug() << "event received: " << event->type();

    // Mouse Released
    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent *me = static_cast<QMouseEvent *>(event);
        this->lastMouseEvent = me;
        this->timerMousePressed->stop();
    }

    // Mouse Press
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *me = static_cast<QMouseEvent *>(event);
        this->lastButtonState = me->button();

        if(this->lastButtonState == Qt::RightButton) {
            this->cameraMode = this->cameraMode?false:true;
            if(this->cameraMode) {
                pCursor->setPos(glw.mapPoint(QPoint((int)this->width/2, (int)this->height/2)));
                glw.setCursor(Qt::BlankCursor);
            } else glw.setCursor(Qt::ArrowCursor);
        }
        if(this->cameraMode) this->timerMousePressed->start();
    }

    // Mouse Moved
    if (event->type() == QEvent::MouseMove) {
        QMouseEvent *me = static_cast<QMouseEvent *>(event);
        if(cameraMode) {
            lastCursorPos = QPoint(me->x(), me->y());
            pCursor->setPos(glw.mapPoint(QPoint((int)this->width/2, (int)this->height/2)));

            // Compute new orientation
            this->horizontalAngle += this->mouseSpeed * float(this->width/2 - lastCursorPos.x());
            this->verticalAngle += this->mouseSpeed * float(this->height/2 - lastCursorPos.y());

            this->calculate();
        }
    }

    // Key Released event
    if (event->type() == 7) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        if(!keyEvent->isAutoRepeat()) {
            switch(keyEvent->key()) {
                case Qt::Key_Shift:
                    speed = 0.15;
                break;
            }
            if(lockKey) {
                switch(keyEvent->key()) {
                    case Qt::Key_W:
                    case Qt::Key_S:
                        lockKey = false;
                        timerKeyPressed->stop();
                    break;
                }
            }
        }

    }

    // Key Pressed event
    if (event->type() == 51) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        if(!keyEvent->isAutoRepeat()) {
            switch(keyEvent->key()) {
                case Qt::Key_Shift:
                    speed = 0.35;
                break;
            }

            if(!lockKey) {
                switch(keyEvent->key()) {
                    case Qt::Key_W:
                    case Qt::Key_S:
                        lockKey = true;
                        lastKey = keyEvent->key();
                        lastKeyEvent = keyEvent;
                        timerKeyPressed->start();
                    break;
                }
            }
        }
    }

}

void Camera::whileKeyPressed() {
    if (this->cameraMode) {
        switch(lastKey) {
            case Qt::Key_W:
                this->moveForwards();
                this->calculate();
            break;
            case Qt::Key_S:
                this->moveForwards(true);
                this->calculate();
            break;
        }
    }
}

void Camera::whileMousePressed() {
    if(this->lastButtonState == Qt::LeftButton) {
        this->moveForwards();
        this->calculate();
    }
}

void Camera::calculate() {
    // Direction : Spherical coordinates to Cartesian coordinates conversion
    direction = glm::vec3 (
        cos(this->verticalAngle) * sin(this->horizontalAngle),
        sin(this->verticalAngle),
        cos(this->verticalAngle) * cos(this->horizontalAngle)
    );

    // Right vector
    glm::vec3 right = glm::vec3(
        sin(this->horizontalAngle - 3.14f/2.0f),
        0,
        cos(this->horizontalAngle - 3.14f/2.0f)
    );

    // Up vector
    glm::vec3 up = glm::cross( right, direction );

    if(this->position.x > this->maxCamera.x) this->position.x = this->maxCamera.x;
    if(this->position.x < -this->maxCamera.x) this->position.x = -this->maxCamera.x;
    if(this->position.z > this->maxCamera.z) this->position.z = this->maxCamera.z;
    if(this->position.z < -this->maxCamera.z) this->position.z = -this->maxCamera.z;
    if(this->position.y > this->maxCamera.y) this->position.y = this->maxCamera.y;
    if(this->position.y < 1) this->position.y = 1;

    // Camera matrix
    this->ProjectionMatrix = glm::perspective(initialFoV, (float)width/(float)height, 1.0f, 3800.0f);
    this->ViewMatrix = glm::lookAt(
                            this->position,           // Camera is here
                            this->position+direction, // and looks here : at the same position, plus "direction"
                            up                  // Head is up (set to 0,-1,0 to look upside-down)
                        );
}
