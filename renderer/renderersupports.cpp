#include <maingl/glrenderer.h>
#include <maingl/glwidget.h>

#include <common/objloaderbuffer.h>
#include <common/textures.h>
#include <common/extrude.h>
#include <common/settings.h>
#include <common/NLSegment.h>

#include "renderersupports.h"
#include <renderer/renderertrack/renderertrackstyles.h>

#include <common/vectorstuff.h>

rendererSupports::rendererSupports(GLWidget& _glw) : glw(_glw) {
    renderer();

    supportObject = new VertexBufferObject();
    supportCapObject = new VertexBufferObject();

    footerScales[0] = 0.20f;
    footerScales[1] = 0.24f;
    footerScales[2] = 0.30f;
    footerScales[3] = 0.50f;

    supportForms[TUBE_SMALL] = NoLimitsRenderer::makeCircle(0.125f, 9, glm::vec3(0.0f,  0.0f, 0.0f));
    supportForms[TUBE_MEDIUM] = NoLimitsRenderer::makeCircle(0.20f, 9, glm::vec3(0.0f,  0.0f, 0.0f));
    supportForms[TUBE_LARGE] = NoLimitsRenderer::makeCircle(0.35f, 9, glm::vec3(0.0f,  0.0f, 0.0f));
    supportForms[TUBE_X_LARGE] = NoLimitsRenderer::makeCircle(0.45f, 9, glm::vec3(0.0f,  0.0f, 0.0f));
    supportForms[TUBE_XX_LARGE] = NoLimitsRenderer::makeCircle(0.60f, 9, glm::vec3(0.0f,  0.0f, 0.0f));
    supportForms[TUBE_H_BEAM] = NoLimitsRenderer::makeCircle(0.0625f, 9, glm::vec3(0.0f,  0.0f, 0.0f));
    supportForms[TUBE_L_BEAM] = NoLimitsRenderer::makeCircle(0.0625f, 9, glm::vec3(0.0f,  0.0f, 0.0f));

    supportsMesh = new Mesh();
    supportsCapMesh = new Mesh();
    connectorsMesh =  new Mesh();
}

void rendererSupports::init() {
    TrackStyles = new rendererTrackStyles();
}

void rendererSupports::render() {
    if(changes) {
        delete supportObject, supportCapObject;
        supportObject = new VertexBufferObject();
        supportObject->setData(supportsMesh);

        supportCapObject = new VertexBufferObject();
        supportCapObject->setData(supportsCapMesh);

        connectorsObject = new VertexBufferObject();
        connectorsObject->setData(connectorsMesh);

        changes = false;
    }
    if(allowRender && NoLimitsRenderer::Settings->rendererSupports) {
        supportObject->drawVBO();
        connectorsObject->drawVBO();

        glDisable(GL_CULL_FACE);
        supportCapObject->drawVBO();
        glEnable(GL_CULL_FACE);
    }
}

glm::vec3 rendererSupports::getSegmentOffset(int segmentIndex, int nodeIndex) {
    NLSegment *segment = new NLSegment(segmentIndex);

    SegmentNode segmentNode = NoLimitsRenderer::Track.seg[segmentIndex].data[nodeIndex];

    int currentSpineStyle = 0;
    currentSpineStyle = (NoLimitsRenderer::Track.segs[segmentIndex].flags & (1 << 2)?2:currentSpineStyle);
    currentSpineStyle = (NoLimitsRenderer::Track.segs[segmentIndex].flags & (1 << 3)?3:currentSpineStyle);
    currentSpineStyle = (!currentSpineStyle?1:currentSpineStyle);
    rendererTrackStyleSpineStyle *spineStyle = currentSpineStyle == 1?(TrackStyle->Spine0):(currentSpineStyle == 2?(TrackStyle->Spine1):(TrackStyle->Spine2));

    glm::vec3 p1 = segment->getOffsetPosition((segment->getLength()*segmentNode.pos));
    bankingValues bankingTNB = segment->getDir(segment->lastOffset);

    glm::vec3 nodePoint = p1 + (bankingTNB.normal * spineStyle->supportOffset);

    glm::mat3 rotation(bankingTNB.tangent, bankingTNB.normal, bankingTNB.binormal);
    glm::mat4 _m;
    _m = glm::translate(_m, p1);
    glm::mat4 rot = _m * glm::mat4(rotation);
    rot = glm::rotate(rot, 180.0f, glm::vec3(0, 0, 1));

    if(spineStyle->useConnector) {
        Mesh *tmpConnectorMesh = new Mesh(*spineStyle->connector->mesh);
        tmpConnectorMesh->applyMatrix(rot);

        // Connector Color
        glm::vec4 tieColor = NoLimitsRenderer::Track.segs[segmentIndex].individualTrackColor?NoLimitsRenderer::makeColor(
           NoLimitsRenderer::Track.segs[segmentIndex].trackCrosstieColor.r,
           NoLimitsRenderer::Track.segs[segmentIndex].trackCrosstieColor.g,
           NoLimitsRenderer::Track.segs[segmentIndex].trackCrosstieColor.b,
           NoLimitsRenderer::Track.segs[segmentIndex].trackCrosstieColor.a
           ):NoLimitsRenderer::makeColor(
           NoLimitsRenderer::Track.trackCrosstieColor.r,
           NoLimitsRenderer::Track.trackCrosstieColor.g,
           NoLimitsRenderer::Track.trackCrosstieColor.b,
           NoLimitsRenderer::Track.trackCrosstieColor.a
        );
        tmpConnectorMesh->setColor(tieColor);

        connectorsMesh->addVerts(tmpConnectorMesh);
        delete tmpConnectorMesh;
    }

    return nodePoint;
}

void rendererSupports::update() {
    //return;

    allowRender = false;

    TrackStyle = TrackStyles->Styles[NoLimitsRenderer::Track.style];
    if(TrackStyle==0) TrackStyle  = TrackStyles->Styles[CLASSIC_STEEL_LOOPING];

    delete supportsMesh, supportsCapMesh, connectorsMesh;
    supportsMesh = new Mesh();
    supportsCapMesh = new Mesh();
    connectorsMesh = new Mesh();

    for( int i = 0; i < NoLimitsRenderer::Track.totalTubes; i++) {
        glm::vec4 p1, p2;

        // Handle Fundament Nodes
        if(NoLimitsRenderer::Track.tubes[i].n1type == NODE_FUND) {
            int fundamentSize = getFundamentSize(NoLimitsRenderer::Track.tubes[i].n1index, &NoLimitsRenderer::Track);
            FundNode node = NoLimitsRenderer::Track.fundNodes[NoLimitsRenderer::Track.tubes[i].n1index];
            p1 = glm::vec4(node.posX, node.posY, node.posZ, 0.0f) + glm::vec4(0.0f, footerScales[fundamentSize], 0.0f, 0.0f);
        }
        if(NoLimitsRenderer::Track.tubes[i].n2type == NODE_FUND) {
            int fundamentSize = getFundamentSize(NoLimitsRenderer::Track.tubes[i].n2index, &NoLimitsRenderer::Track);
            FundNode node = NoLimitsRenderer::Track.fundNodes[NoLimitsRenderer::Track.tubes[i].n2index];
            p2 = glm::vec4(node.posX, node.posY, node.posZ, 0.0f) + glm::vec4(0.0f, footerScales[fundamentSize], 0.0f, 0.0f);
        }

        // Handle Free Nodes
        if(NoLimitsRenderer::Track.tubes[i].n1type == NODE_FREE) {
            FreeNode node = NoLimitsRenderer::Track.freeNodes[NoLimitsRenderer::Track.tubes[i].n1index];
            p1 = glm::vec4(node.posX, node.posY, node.posZ, 0.0f);
        }

        if(NoLimitsRenderer::Track.tubes[i].n2type == NODE_FREE) {
            FreeNode node = NoLimitsRenderer::Track.freeNodes[NoLimitsRenderer::Track.tubes[i].n2index];
            p2 = glm::vec4(node.posX, node.posY, node.posZ, 0.0f);
        }

        // Handle Segment Nodes
        if(NoLimitsRenderer::Track.tubes[i].n1type == NODE_SEGMENT) {
            glm::vec3 node = getSegmentOffset(NoLimitsRenderer::Track.tubes[i].n1segIndex, NoLimitsRenderer::Track.tubes[i].n1index);
            p1 = glm::vec4(node.x, node.y, node.z, 0.0f);
        }
        // Handle Segment Nodes
        if(NoLimitsRenderer::Track.tubes[i].n2type == NODE_SEGMENT) {
            glm::vec3 node = getSegmentOffset(NoLimitsRenderer::Track.tubes[i].n2segIndex, NoLimitsRenderer::Track.tubes[i].n2index);
            p2 = glm::vec4(node.x, node.y, node.z, 0.0f);
        }

        int customColor = NoLimitsRenderer::Track.tubes[i].flags & (1 << 3)?true:false;

        glm::vec4 supportColor = customColor?NoLimitsRenderer::makeColor(
           NoLimitsRenderer::Track.tubes[i].color.r,
           NoLimitsRenderer::Track.tubes[i].color.g,
           NoLimitsRenderer::Track.tubes[i].color.b,
           NoLimitsRenderer::Track.tubes[i].color.a
           ):NoLimitsRenderer::makeColor(
           NoLimitsRenderer::Track.supportsColor.r,
           NoLimitsRenderer::Track.supportsColor.g,
           NoLimitsRenderer::Track.supportsColor.b,
           NoLimitsRenderer::Track.supportsColor.a
           );

        if(p1.x == p2.x) p2.x += 0.0001f;
        if(p1.z == p2.z) p2.z += 0.0001f;

        vec4List _supportPoints;
        _supportPoints.push_back(p1);
        _supportPoints.push_back(p2);
        vec4List _supportForm =  supportForms[NoLimitsRenderer::Track.tubes[i].tubeType];
        if(!_supportForm.size()) continue;
        extrude *_support = new extrude(_supportPoints, _supportForm, false);

        _support->mesh->setColor(supportColor);
        _support->capMesh->setColor(supportColor);

        supportsMesh->addVerts(new Mesh(*_support->mesh));
        supportsCapMesh->addVerts(new Mesh(*_support->capMesh));
    }

    changes = true;
    allowRender = true;
}
