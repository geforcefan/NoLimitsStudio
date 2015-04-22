#include <maingl/glrenderer.h>
#include <maingl/glwidget.h>

#include <common/objloaderbuffer.h>
#include <common/textures.h>
#include <common/extrude.h>
#include <common/settings.h>

#include "rendererfooter.h"

rendererFooter::rendererFooter(GLWidget& _glw) : glw(_glw) {
    renderer();

    baseObject = new VertexBufferObject();
    plateTopObject = new VertexBufferObject();
    plateBottomObject = new VertexBufferObject();
    screwsObject = new VertexBufferObject();

    baseObjectMesh = new Mesh();
    plateTopObjectMesh = new Mesh();
    plateBottomObjectMesh = new Mesh();
    screwsObjectMesh = new Mesh();

    footerScales[0] = 0.6f;
    footerScales[1] = 1.0f;
    footerScales[2] = 2.0f;
    footerScales[3] = 2.5f;

    footerPosition[0] = glm::vec3(0.0f, 0.2f, 0.0f);
    footerPosition[1] = glm::vec3(0.0f, 0.2f, 0.0f);
    footerPosition[2] = glm::vec3(0.0f, 0.2f, 0.0f);
    footerPosition[3] = glm::vec3(0.0f, 0.2f, 0.0f);
}

void rendererFooter::init() {
    textureBase = NoLimitsRenderer::Textures->loadTexture("rendererFooter:baseObject", ":/models/Footer/Base.jpg");

    _baseObject = new objLoaderBuffer(":/models/Footer.obj", "Base");
    _plateTopObject = new objLoaderBuffer(":/models/Footer.obj", "PlateTop");
    _plateBottomObject = new objLoaderBuffer(":/models/Footer.obj", "PlateBottom");
    _screwsObject = new objLoaderBuffer(":/models/Footer.obj", "Screw[.](.*)");
}

void rendererFooter::render() {
    if(changes) {
        baseObject->setData(baseObjectMesh);
        plateTopObject->setData(plateTopObjectMesh);
        plateBottomObject->setData(plateBottomObjectMesh);
        screwsObject->setData(screwsObjectMesh);
    }
    if(allowRender && NoLimitsRenderer::Settings->rendererSupports) {
        NoLimitsRenderer::Textures->bindTexture(textureBase);
        baseObject->drawVBO();
        NoLimitsRenderer::Textures->releaseTexture();
        plateTopObject->drawVBO();
        plateBottomObject->drawVBO();
        screwsObject->drawVBO();
        changes = false;
    }
}

void rendererFooter::update() {
    allowRender = false;

    delete baseObject, plateTopObject, plateBottomObject, screwsObject;
    baseObject = new VertexBufferObject();
    plateTopObject = new VertexBufferObject();
    plateBottomObject = new VertexBufferObject();
    screwsObject = new VertexBufferObject();

    delete baseObjectMesh, plateTopObjectMesh, plateBottomObjectMesh, screwsObjectMesh;
    baseObjectMesh = new Mesh();
    plateTopObjectMesh = new Mesh();
    plateBottomObjectMesh = new Mesh();
    screwsObjectMesh = new Mesh();

    for(int i=0; i < NoLimitsRenderer::Track.totalFundNodes; i++) {
        int fundamentSize = getFundamentSize(i, &NoLimitsRenderer::Track);

        glm::vec3 nodePosition(
                    NoLimitsRenderer::Track.fundNodes[i].posX,
                    NoLimitsRenderer::Track.fundNodes[i].posY,
                    NoLimitsRenderer::Track.fundNodes[i].posZ);

        glm::mat4 objMatrix = glm::mat4(1.0f);
        objMatrix = glm::translate(objMatrix, nodePosition);
        objMatrix = glm::translate(objMatrix, footerPosition[fundamentSize]);
        objMatrix = glm::scale(objMatrix, glm::vec3(footerScales[fundamentSize]));
        objMatrix = glm::rotate(objMatrix, (float)((float)NoLimitsRenderer::Track.fundNodes[i].rotation / (M_PI / 180.0f)), glm::vec3(0.0f, 1.0f, 0.0f));

        Mesh *_baseObjectMesh = new Mesh(*_baseObject->mesh);
        _baseObjectMesh->applyMatrix(objMatrix);
        Mesh *_plateTopObjectMesh = new Mesh(*_plateTopObject->mesh);
        _plateTopObjectMesh->applyMatrix(objMatrix);
        Mesh *_plateBottomObjectMesh = new Mesh(*_plateBottomObject->mesh);
        _plateBottomObjectMesh->applyMatrix(objMatrix);
        Mesh *_screwsObjectMesh = new Mesh(*_screwsObject->mesh);
        _screwsObjectMesh->applyMatrix(objMatrix);

        NoLimitsRenderer::setColor(glm::vec4(1.0f));
        baseObjectMesh->addVerts(new Mesh(*_baseObjectMesh));

        int customColor = NoLimitsRenderer::Track.fundNodes[i].flags & (1 << 3)?true:false;
        glm::vec4 plateTopColor = customColor?NoLimitsRenderer::makeColor(
           NoLimitsRenderer::Track.fundNodes[i].color.r,
           NoLimitsRenderer::Track.fundNodes[i].color.g,
           NoLimitsRenderer::Track.fundNodes[i].color.b,
           NoLimitsRenderer::Track.fundNodes[i].color.a
           ):NoLimitsRenderer::makeColor(
           NoLimitsRenderer::Track.supportsColor.r,
           NoLimitsRenderer::Track.supportsColor.g,
           NoLimitsRenderer::Track.supportsColor.b,
           NoLimitsRenderer::Track.supportsColor.a
           );

        _plateTopObjectMesh->setColor(plateTopColor);
        plateTopObjectMesh->addVerts(new Mesh(*_plateTopObjectMesh));

        NoLimitsRenderer::setColor(glm::vec4(1.0f));
        plateBottomObjectMesh->addVerts(new Mesh(*_plateBottomObjectMesh));
        screwsObjectMesh->addVerts(new Mesh(*_screwsObjectMesh));

        delete _baseObjectMesh, _plateTopObjectMesh, _plateBottomObjectMesh, _screwsObjectMesh;
    }

    //delete _baseObject, plateTopObjectMesh, plateBottomObjectMesh, screwsObjectMesh;

    changes = true;
    allowRender = true;
}
