#include <common/global.h>
#include "eventhandler.h"

#include <QDebug>
#include <QEvent>
#include <QKeyEvent>

#include "trackbuilder.h"
#include "common/trackcamera.h"

EventHandler::EventHandler(QObject *parent) :
    QObject(parent)
{
    usePov = false;
    pCursor = new QCursor();
}

void EventHandler::receiveEvent(QObject *obj, QEvent *event) {
    if(event->type() == QEvent::Resize) {
        emit windowResize();
    }

    if (event->type() == 51) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        switch(keyEvent->key()) {
            case Qt::Key_Space:
            if(!usePov) {
                NoLimitsRenderer::bindCamera(trackBuilder->trackCamera);
                usePov = true;
                //pCursor->setPos(NoLimitsRenderer::camera->glw.mapPoint(QPoint((int)NoLimitsRenderer::camera->width/2, (int)NoLimitsRenderer::camera->height/2)));
                //NoLimitsRenderer::camera->glw.setCursor(Qt::BlankCursor);
            } else {
                NoLimitsRenderer::restoreCamera();
                usePov = false;
                //NoLimitsRenderer::camera->glw.setCursor(Qt::ArrowCursor);
            }
            break;
        }
    }
}
