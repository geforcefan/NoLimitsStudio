#include <common/global.h>

#include <QtGui>

#include <common/camera.h>
#include <common/textures.h>
#include <common/settings.h>
#include <maingl/glrenderer.h>
#include <QGLShader>

#include "rendererenvironment.h"

#include <iostream>
#include <fstream>
#include <istream>
#include <sstream>

rendererEnvironment::rendererEnvironment(GLWidget& _glw) : glw(_glw) {
    renderer();
    size = glm::vec2(900, 900);

    groundMesh = new Mesh();
}

void rendererEnvironment::init() {
    textureGrass = NoLimitsRenderer::Textures->loadTexture("rendererEnvironment:grass", ":/grass2.jpg");

    //delete groundMesh;
    groundMesh = new Mesh();

    // skyDomeSphere
    skyDomeSphere = new objLoaderBuffer(":/models/Sky.obj", "Kugel");
    skyDomeSphere->finalizeData();

    // Flat Ground
    glm::vec4 color = glm::vec4(1.0,1.0,1.0,1.0);

    groundMesh->addPoint(glm::vec3(-size.x, 0.0f,  size.y), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0, 130), color);
    groundMesh->addPoint(glm::vec3( size.x, 0.0f,  size.y), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(130, 130), color);
    groundMesh->addPoint(glm::vec3( size.x, 0.0f, -size.y), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(130, 0), color);


    groundMesh->addPoint(glm::vec3( size.x, 0.0f, -size.y), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(130, 0), color);
    groundMesh->addPoint(glm::vec3(-size.x, 0.0f, -size.y), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0, 0), color);
    groundMesh->addPoint(glm::vec3(-size.x, 0.0f,  size.y), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0, 130), color);

    vboObject = new VertexBufferObject();
    vboObject->setData(groundMesh);
}

void rendererEnvironment::update() {
    changes = true;
}

void rendererEnvironment::render() {
    if(changes) {
        delete vboObject;
        vboObject = new VertexBufferObject();
        if(!NoLimitsRenderer::Track.sizeX) vboObject->setData(groundMesh);
        changes = false;
    }
    if(!NoLimitsRenderer::Settings->rendererCadMode && NoLimitsRenderer::Settings->rendererSky) {
        glUniform1i(NoLimitsRenderer::currentShader->uniformLocation("useScattering") , 1);
        glCullFace(GL_FRONT);
        skyDomeSphere->drawVBO();
        glCullFace(GL_BACK);
        glUniform1i(NoLimitsRenderer::currentShader->uniformLocation("useScattering") , 0);
    }

    if(NoLimitsRenderer::Settings->rendererGround) {
        NoLimitsRenderer::Textures->bindTexture(textureGrass);
        vboObject->drawVBO();
        NoLimitsRenderer::Textures->releaseTexture();
    }
}
