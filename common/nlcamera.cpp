#include "NLCamera.h"

#include <common/global.h>

#include <maingl/glwidget.h>
#include <QEvent>
#include <QKeyEvent>
#include <QTimer>
#include <QObject>

#include <QDebug>
#include <QApplication>

#include "common/camerabase.h"
#include "track/mnode.h"
#include "trackbuilder.h"

#include <common/nlbanking.h>

NLCamera::NLCamera(GLWidget &_glw): CameraBase(_glw) {
    speed = 1.0f; // 3 units / second

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
    timerKeyPressed = new QTimer();
    timerKeyPressed->setInterval(5);
    QObject::connect(this->timerKeyPressed, SIGNAL(timeout()), this, SLOT(whileKeyPressed()));
    lockKey = false;

    povPos = 0;
}

void NLCamera::moveForwards()
{
    this->moveForwards(false);
}

void NLCamera::moveForwards(bool reverse, bool phantom)
{

    glm::mat4 anchorBase;
    mnode *povNode;

    povPos += !phantom?(speed*5*(reverse?-1:1)):0;
    if(NoLimitsRenderer::TrackPOV->lNodes.size())
    {
        if(povPos < 0)
        {
            povPos += NoLimitsRenderer::TrackPOV->lNodes.size();
        }
        if(povPos > NoLimitsRenderer::TrackPOV->lNodes.size() - 1) {
            povPos = 0;
            povNode = NoLimitsRenderer::TrackPOV->lNodes[povPos];
        } else {
            povNode = NoLimitsRenderer::TrackPOV->lNodes[povPos];

        }
            anchorBase = glm::translate(trackBuilder->BuilderTrack->startPos) * glm::rotate(trackBuilder->BuilderTrack->startYaw, glm::vec3(0.f, 1.f, 0.f));

            ViewMatrix = glm::lookAt(
                        glm::vec3(glm::vec4(povNode->vPos+(povNode->vNorm*-0.6f), 1.f)),
                        glm::vec3(glm::vec4(povNode->vPos+(povNode->vNorm*-0.6f)+povNode->vDirHeart(1.0f), 1.f)),
                        -glm::vec3(glm::vec4(povNode->vNorm, 0.f))
                        );
    } else {
        direction = glm::vec3 (cos(this->verticalAngle) * sin(this->horizontalAngle), sin(this->verticalAngle), cos(this->verticalAngle) * cos(this->horizontalAngle));
        glm::vec3 right = glm::vec3(sin(this->horizontalAngle - 3.14f/2.0f), 0, cos(this->horizontalAngle - 3.14f/2.0f));
        glm::vec3 up = glm::cross( right, direction );
        ViewMatrix = glm::lookAt(trackBuilder->BuilderTrack->startPos, trackBuilder->BuilderTrack->startPos+direction, up);
    }

    ProjectionMatrix = glm::perspective(89.0f, (float)width/(float)height, 0.1f, 1000.0f);
}

bool NLCamera::eventFilterFake(QObject *obj, QEvent *event) {
    //if(event->type() != 5) qDebug() << "event received: " << event->type();

    // Key Released event
    if (event->type() == 7) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        if(!keyEvent->isAutoRepeat()) {
            switch(keyEvent->key()) {
                case Qt::Key_Shift:
                    speed = 1.0;
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
                    speed = 2.0;
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


    if (event->type() == QEvent::MouseMove) {
        //pCursor->setPos(glw.mapPoint(QPoint((int)this->width/2, (int)this->height/2)));
    }

}

void NLCamera::whileKeyPressed() {
    if (this->cameraMode || 1) {
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

void NLCamera::calculate() {
    moveForwards(false, true);
}
