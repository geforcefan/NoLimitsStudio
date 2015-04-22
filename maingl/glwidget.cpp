#include <common/global.h>
#include <qcolordialog.h>

#include <maingl/glwidget.h>
#include <maingl/glthread.h>

#include <common/plugins.h>

#include <renderer/renderer.h>
#include <vector>

#include <QDebug>

GLWidget::GLWidget(const QGLFormat& format, QWidget *parent, const char *name)
    : QGLWidget(parent, this) ,
    glt(*this) {
        QGLFormat f = QGLFormat::defaultFormat();
        f.setSampleBuffers(true);
        f.setOverlay(false);
        this->setFormat(f);
        setMouseTracking(true);
        setAttribute(Qt::WA_OpaquePaintEvent,true);
        this->setAutoBufferSwap(true);
    }


void GLWidget::mouseMoveEvent(QMouseEvent *e) {
}

QPoint GLWidget::mapPoint(QPoint p) {
    return this->mapToGlobal(p);
}

bool GLWidget::eventFilterFake(QObject *obj, QEvent *event) {
  return false;
}

void GLWidget::initRendering() {
}

void GLWidget::finishRendering() {
}

void GLWidget::closeEvent( QCloseEvent * _e ) {
    QGLWidget::closeEvent(_e);
}

void GLWidget::paintEvent( QPaintEvent * ) {
}

void GLWidget::resizeEvent( QResizeEvent * _e ) {
}

void GLWidget::lockGLContext( ) {
}

void GLWidget::unlockGLContext( ) {
}

void GLWidget::render() {}

GLThread & GLWidget::getGLThread() {
    return glt;
}

QWaitCondition & GLWidget::renderCondition( ) {
    return(render_condition);
}

QMutex & GLWidget::renderMutex( ) {
    return(render_mutex);
}



