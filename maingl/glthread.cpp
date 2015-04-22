#include <common/global.h>
#include <maingl/glthread.h>
#include <maingl/glwidget.h>
#include <common/camera.h>
#include <common/plugins.h>
#include <common/lights.h>

// Renderer
#include <maingl/glrenderer.h>
#include <renderer/rendererenvironment.h>
#include <renderer/renderertrack.h>
#include <renderer/renderer.h>

#include <QGLFormat>
#include <QDebug>
#include <QGLShader>
#include <common/textures.h>
#include <common/nlcamera.h>

#include <iostream>

GLThread::GLThread( GLWidget & _glw )
        : QThread(),
        glw(_glw),
        render_flag(true),
        resize_flag(false),
        viewport_size(_glw.size()) {

    // FPS Calculator
    window = (QMainWindow*)glw.parent();
    numFrames = 0;
    m_fpsTimer = new QTimer();
    GLThread::connect( this->m_fpsTimer, SIGNAL( timeout() ), this, SLOT( showFramesPerSecond() ) );
    m_fpsTimer->start( 125 );

    // Init Camera
    NoLimitsRenderer::camera = new Camera(this->glw);

    // Init Renderer
    Renderer = new GLRenderer(this);
    NoLimitsRenderer::glrenderer = Renderer;

    running = false;
}

void GLThread::resizeViewport( const QSize& _size ) {
    if(!running) return;
    // set size and flag to request resizing
    this->viewport_size = _size;
    this->resize_flag = true;

    glViewport(0, 0, _size.width(), _size.height());
    NoLimitsRenderer::camera->setDimensions(_size.width(), _size.height());
    Renderer->resize(_size.width(), _size.height());
    glw.render();
}

void GLThread::initializeGL() {
    if(!running) return;
    qDebug("OpenGL Version: %s", glGetString(GL_VERSION));
    qDebug("OpenGL Shader Version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

    Renderer->init();
    NoLimitsRenderer::Plugins->initGL();

    glClear( GL_COLOR_BUFFER_BIT |  GL_DEPTH_BUFFER_BIT);
}

void GLThread::init() {
    if(!running) return;
    glw.makeCurrent();
    Renderer->update();
}

void GLThread::resizeGL(int width, int height) {
    if(!running) return;
    glViewport(0, 0, width, height);
    NoLimitsRenderer::camera->setDimensions(width, height);
    Renderer->resize(width, height);
    glw.render();
}

void GLThread::paintGL() {
    if(!running) return;
    ++numFrames;

    glClear( GL_COLOR_BUFFER_BIT |  GL_DEPTH_BUFFER_BIT);
    Renderer->draw();
}

bool GLThread::eventFilterFake(QObject *obj, QEvent *event) {
    NoLimitsRenderer::camera->eventFilterFake(obj, event);
}

void GLThread::showFramesPerSecond() {
    float fps = float( numFrames ) * 8;
    //window->setWindowTitle(QString("%1 FPS").arg(fps));
    numFrames = 0;
}

void GLThread::stop( ) {
    this->render_flag = false;
    glw.lockGLContext();
    quit();
}

void GLThread::run( ) {
    running = true;

    // Init Renderer
    Renderer = new GLRenderer(this);
    NoLimitsRenderer::glrenderer = Renderer;

    resize_flag = true;
    //glw.makeCurrent();

    this->initializeGL();

    while (render_flag) {
        if (resize_flag) {
            this->resizeGL(this->viewport_size.width(), this->viewport_size.height());
            //resize_flag = false;
        }

        paintGL();

        glw.updateGL();
        msleep(16);
    }
}
