#include <common/global.h>
#include <maingl/glthread.h>
#include <common/shader.h>
#include <common/camera.h>
#include <common/lights.h>
#include <common/textures.h>
#include <common/plugins.h>
#include <window/windowmain.h>
#include <common/nlbanking.h>

#include <renderer/renderer.h>
#include <renderer/rendererenvironment.h>
#include <renderer/renderertrack.h>
#include <renderer/rendererfooter.h>
#include <renderer/renderersupports.h>
#include <renderer/rendererterrain.h>

#include <QThread>
#include <QDebug>

#include "glrenderer.h"

GLRendererThread::GLRendererThread(QObject *parent, GLRenderer *_glr, int _rendererIndex) : QThread(parent), glr(_glr), rendererIndex(_rendererIndex) {}
GLRendererThread::~GLRendererThread() {}
void GLRendererThread::run() {
    glr->Renderer[rendererIndex]->update();
    quit();
}
GLRenderer::GLRenderer(GLThread *_glt) : glt(_glt), QObject(_glt)
{
    // Init Renderer
    this->Renderer.push_back(new rendererEnvironment(glt->glw));
    this->Renderer.push_back(new rendererFooter(glt->glw));
    this->Renderer.push_back(new rendererSupports(glt->glw));
    this->Renderer.push_back(new rendererTerrain(glt->glw));
    this->Renderer.push_back(new rendererTrack(&NoLimitsRenderer::Track, glt->glw));

    this->numRenderer = this->Renderer.size();

    // Get sizes
    this->width = glt->viewport_size.width();
    this->height = glt->viewport_size.height();

    // GL Renderer Thread
    allowRender = true;
    for(int i=0; i < numRenderer; i++) glrt.push_back(new GLRendererThread(this, this, i));
}

void GLRenderer::addRenderer(renderer *rnd) {
    this->Renderer.push_back(rnd);
    glrt.push_back(new GLRendererThread(this, this, numRenderer));
    this->Renderer[numRenderer]->init();
    numRenderer++;
}

void GLRenderer::init() {
    glEnable(GL_LIGHTING);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    /*glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);*/

    // Main Shader
#ifdef Q_OS_MAC
    //MainShader = new shader(":/osxMain.vert", ":/osxMain.frag");
    MainShader = new shader("/Users/ercanakyurek/Daten/NoLimitsTools/nlstudio/shaders/osxMain.vert", "/Users/ercanakyurek/Daten/NoLimitsTools/nlstudio/shaders/osxMain.frag");
#endif
#ifdef Q_OS_WIN
    MainShader = new shader("/Users/ercanakyurek/Desktop/nlstudio/shaders/main.vert", "/Users/ercanakyurek/Desktop/nlstudio/shaders/main.frag");
#endif
    NoLimitsRenderer::currentShader = MainShader;

    NoLimitsRenderer::bindShader(MainShader);

    NoLimitsRenderer::initGL();

    // Initalize sub renderer
    for(int i=0; i < this->numRenderer; i++) this->Renderer[i]->init();

    update();

    NoLimitsRenderer::mainWindow->initContent();

    glt->glw.doneCurrent();
}

void GLRenderer::resize(float _width, float _height) {
    // Get sizes
    this->width = _width;
    this->height = _height;
}

void GLRenderer::update() {
    NoLimitsRenderer::TrackBanking = new NLBanking();
    NoLimitsRenderer::TrackBanking->update();
    NoLimitsRenderer::TrackPOV->update();

    NoLimitsRenderer::camera->position = glm::vec3(NoLimitsRenderer::Track.beziers[0].posX,
                                      NoLimitsRenderer::Track.beziers[0].posY + 2.0f,
                                      NoLimitsRenderer::Track.beziers[0].posZ);
    NoLimitsRenderer::camera->calculate();
    glt->glw.render();

    for(int i=0; i < glrt.size(); i++) glrt[i]->start();
}

void GLRenderer::drawObjects() {
    glt->glw.makeCurrent();
    for(int i=0; i < Renderer.size(); i++) Renderer[i]->render();
}

void GLRenderer::drawScene() {
    glViewport(0, 0, width, height);

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.62, 0.87, 1, 1);

    NoLimitsRenderer::bindShader(MainShader);

    glm::mat4 projMatrix = NoLimitsRenderer::camera->getProjectionMatrix();
    glm::mat4 viewMatrix = NoLimitsRenderer::camera->getViewMatrix();

    glUniformMatrix4fv(NoLimitsRenderer::currentShader->uniformLocation("projMatrix") , 1, GL_FALSE, glm::value_ptr(projMatrix));
    glUniformMatrix4fv(NoLimitsRenderer::currentShader->uniformLocation("viewMatrix") , 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniform3fv(NoLimitsRenderer::currentShader->uniformLocation("cameraPosition") , 1, glm::value_ptr(NoLimitsRenderer::camera->getPosition()));

    NoLimitsRenderer::Lights->setUniforms(0);

    glCullFace(GL_BACK);
    drawObjects();

    NoLimitsRenderer::currentShader->release();
}

void GLRenderer::draw() {
    glt->glw.makeCurrent();

    drawScene();

    NoLimitsRenderer::currentShader->release();

    glt->glw.doneCurrent();
}
