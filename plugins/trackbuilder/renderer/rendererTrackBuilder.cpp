#include <maingl/glrenderer.h>
#include <maingl/glwidget.h>

#include <common/objloaderbuffer.h>
#include <common/textures.h>
#include <common/extrude.h>
#include <common/vectorstuff.h>
#include <common/vertexbufferobjectextended.h>
#include <common/settings.h>

#include "rendererTrackBuilder.h"
#include "trackbuilder.h"

#include <QDebug>

rendererTrackBuilder::rendererTrackBuilder() {
    renderer();

    Mutex = new QMutex();

    // Heartline Object
    heartlineMesh = new Mesh();
    heartlineObject = new VertexBufferObjectExtended();

    // Tmp Object
    tmpHeartlineMesh = new Mesh();

    // New try of track
    trackRailLeftMesh = new Mesh();
    trackRailRightMesh = new Mesh();
    trackRailBottomMesh = new Mesh();
    trackTieMesh = new Mesh();
    trackRailLeftMeshTmp = new Mesh();
    trackRailRightMeshTmp = new Mesh();
    trackRailBottomMeshTmp = new Mesh();
    trackTieMeshTmp = new Mesh();
    trackRailLeftObject = new VertexBufferObjectExtended();
    trackRailRightObject = new VertexBufferObjectExtended();
    trackRailBottomObject = new VertexBufferObjectExtended();


    trackBoneMeshTmp = new Mesh();
    trackBoneMesh = new Mesh();
    trackBoneMeshObject = new VertexBufferObjectExtended();

    trackRailMeshTmp = new Mesh();
    trackRailMesh = new Mesh();
    trackRailMeshObject = new VertexBufferObjectExtended();


    trackTieMeshTmp = new Mesh();
    trackTieMesh = new Mesh();
    trackTieMeshObject = new VertexBufferObjectExtended();

    useHeat = false;

    QVector<glm::vec2> pointsBone;
    vec4List c1 = NoLimitsRenderer::makeCircle(0.0625f, 12, glm::vec3(0.6f, 0.0f, 0.0f));
    vec4List c2 = NoLimitsRenderer::makeCircle(0.0625f, 12, glm::vec3(-0.6f, 0.0f, 0.0f));
    vec4List c3 = NoLimitsRenderer::makeSquare(glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(-0.45f, -0.0f, 0.0f));
    //vec4List c3 = NoLimitsRenderer::makeFormFormFile(":/trackstyles/FLOORLESS_TWISTED_STEEL_COASTER/SpineForm.txt");
    //vec4List c3 = NoLimitsRenderer::makeSquare(glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

    QVector<glm::vec2> pointsC1;
    QVector<glm::vec2> pointsC2;
    QVector<glm::vec2> pointsC3;

    for(int i = 0; i < c1.size(); i++) pointsC1.append(glm::vec2(c1[i].x, -c1[i].y));
    for(int i = 0; i < c2.size(); i++) pointsC2.append(glm::vec2(c2[i].x, -c2[i].y));
    for(int i = 0; i < c3.size(); i++) pointsC3.append(glm::vec2(-c3[i].y, -c3[i].x));

    pointsRail.append(pointsC1);
    pointsRail.append(pointsC2);
    pointsTrack.append(pointsC3);

    rendererTrackStyles *TrackStyles = new rendererTrackStyles();
    TrackStyle = TrackStyles->Styles[FLOORLESS_TWISTED_STEEL_COASTER];
    currentSpineStyle = new rendererTrackStyleSpineStyle();
    currentSpineStyle = TrackStyle->Spine0;
}

void rendererTrackBuilder::init() {
    NoLimitsRenderer::setColor(glm::vec4(1.0f));

    changes = false;
    allowRender = false;
}

void rendererTrackBuilder::updateHeatValues() {
    useHeat = false;
    if(trackBuilder->visualizeForces) {
        useHeat = true;
        heatMinValue = -1.0f;
        heatMaxValue = 6.0f;
        heatMinVisibleWaveLength = 350.0f;
        heatMaxVisibleWaveLength = 700.0f;
    }
}

void rendererTrackBuilder::rebuildHeartlineAndTrack() {
    delete tmpHeartlineMesh;
    tmpHeartlineMesh = new Mesh();

    delete trackRailLeftMeshTmp, trackRailRightMeshTmp, trackRailBottomMeshTmp, trackTieMeshTmp;
    trackRailLeftMeshTmp = new Mesh();
    trackRailRightMeshTmp = new Mesh();
    trackRailBottomMeshTmp = new Mesh();
    trackTieMeshTmp = new Mesh();

    delete trackBoneMeshTmp;
    trackBoneMeshTmp = new Mesh();

    delete trackRailMeshTmp;
    trackRailMeshTmp = new Mesh();


    delete trackTieMeshTmp;
    trackTieMeshTmp = new Mesh();

    float distFromLastNode = 0.1f;

    track* Track = trackBuilder->BuilderTrack;
    float fSpine = Track->fHeart < 0 ? -0.2f : 0.2f;

    glm::vec4 colorTrack = glm::vec4(0.5f);
    glm::vec4 colorHeartline = glm::vec4(0.0f, 0.78f, 1.0f, 1.0f);

    std::map<int, qvector3d> lastPointRail;

    for(int i = 0; i < Track->lSections.size(); i++)
    {
        section* curSection = Track->lSections[i];
        for(int j = 0; j < curSection->lNodes.size(); ++j)
        {
            distFromLastNode += curSection->lNodes.at(j)->fDistFromLast;
            if(distFromLastNode >= 0.1f || j == curSection->lNodes.size()-1)
            {
                if(distFromLastNode >= 0.1f)
                {
                    distFromLastNode-=0.1f;
                }
                else
                {
                    distFromLastNode = 0.f;
                }

                for(int t=0; t<pointsRail.size(); t++) {
                    qvector3d currentPointRail;

                    for(int k=0; k < pointsRail[t].size(); k++) {
                        currentPointRail.append(glm::vec3(
                            curSection->lNodes.at(j)->fPosHeartx(Track->fHeart+pointsRail[t][k].y)+curSection->lNodes.at(j)->vLatHeart(Track->fHeart).x*pointsRail[t][k].x,
                            curSection->lNodes.at(j)->fPosHearty(Track->fHeart+pointsRail[t][k].y)+curSection->lNodes.at(j)->vLatHeart(Track->fHeart).y*pointsRail[t][k].x,
                            curSection->lNodes.at(j)->fPosHeartz(Track->fHeart+pointsRail[t][k].y)+curSection->lNodes.at(j)->vLatHeart(Track->fHeart).z*pointsRail[t][k].x
                        ));
                    }

                    if(currentPointRail.size()>1 && lastPointRail[t].size()) for(int k=0; k < currentPointRail.size(); k++) {
                        int nextIndex = k==currentPointRail.size()-1?0:k+1;

                        glm::vec3 u = currentPointRail[nextIndex]-lastPointRail[t][k];
                        glm::vec3 v = currentPointRail[k]-lastPointRail[t][k];
                        glm::vec3 nms = glm::normalize(glm::cross(u,v));

                        trackRailMeshTmp->addPoint(lastPointRail[t][k], nms, glm::vec2(1.0), colorTrack);
                        trackRailMeshTmp->addPoint(currentPointRail[nextIndex], nms, glm::vec2(1.0), colorTrack);
                        trackRailMeshTmp->addPoint(currentPointRail[k], nms, glm::vec2(1.0), colorTrack);

                        u = lastPointRail[t][nextIndex]-lastPointRail[t][k];
                        v = currentPointRail[nextIndex]-lastPointRail[t][k];
                        nms = glm::normalize(glm::cross(u,v));

                        trackRailMeshTmp->addPoint(lastPointRail[t][k], nms, glm::vec2(1.0), colorTrack);
                        trackRailMeshTmp->addPoint(lastPointRail[t][nextIndex], nms, glm::vec2(1.0), colorTrack);
                        trackRailMeshTmp->addPoint(currentPointRail[nextIndex], nms, glm::vec2(1.0), colorTrack);
                    }

                    lastPointRail[t] = currentPointRail;
                }

            }
        }
    }

    std::map<int, qvector3d> lastPointBones;

    for(int i = 0; i < Track->lSections.size(); i++)
    {
        section* curSection = Track->lSections[i];
        for(int j = 0; j < curSection->lNodes.size(); ++j)
        {
            distFromLastNode += curSection->lNodes.at(j)->fDistFromLast;
            if(distFromLastNode >= 1.4f || j == curSection->lNodes.size()-1)
            {
                if(distFromLastNode >= 1.4f)
                {
                    distFromLastNode-=1.4f;
                }
                else
                {
                    distFromLastNode = 0.f;
                }


                /*glm::vec3 left = glm::vec3(
                            curSection->lNodes.at(j)->fPosHeartx(Track->fHeart)-curSection->lNodes.at(j)->vLatHeart(Track->fHeart).x*0.55,
                            curSection->lNodes.at(j)->fPosHearty(Track->fHeart)-curSection->lNodes.at(j)->vLatHeart(Track->fHeart).y*0.55,
                            curSection->lNodes.at(j)->fPosHeartz(Track->fHeart)-curSection->lNodes.at(j)->vLatHeart(Track->fHeart).z*0.55
                        );

                glm::vec3 leftmiddle = glm::vec3(
                            curSection->lNodes.at(j)->fPosHeartx(Track->fHeart+fSpine)-curSection->lNodes.at(j)->vLatHeart(Track->fHeart).x*0.35,
                            curSection->lNodes.at(j)->fPosHearty(Track->fHeart+fSpine)-curSection->lNodes.at(j)->vLatHeart(Track->fHeart).y*0.35,
                            curSection->lNodes.at(j)->fPosHeartz(Track->fHeart+fSpine)-curSection->lNodes.at(j)->vLatHeart(Track->fHeart).z*0.35
                        );

                glm::vec3 right = glm::vec3(
                            curSection->lNodes.at(j)->fPosHeartx(Track->fHeart)+curSection->lNodes.at(j)->vLatHeart(Track->fHeart).x*0.55,
                            curSection->lNodes.at(j)->fPosHearty(Track->fHeart)+curSection->lNodes.at(j)->vLatHeart(Track->fHeart).y*0.55,
                            curSection->lNodes.at(j)->fPosHeartz(Track->fHeart)+curSection->lNodes.at(j)->vLatHeart(Track->fHeart).z*0.55
                        );


                glm::vec3 rightmiddle = glm::vec3(
                            curSection->lNodes.at(j)->fPosHeartx(Track->fHeart+fSpine)+curSection->lNodes.at(j)->vLatHeart(Track->fHeart).x*0.35,
                            curSection->lNodes.at(j)->fPosHearty(Track->fHeart+fSpine)+curSection->lNodes.at(j)->vLatHeart(Track->fHeart).y*0.35,
                            curSection->lNodes.at(j)->fPosHeartz(Track->fHeart+fSpine)+curSection->lNodes.at(j)->vLatHeart(Track->fHeart).z*0.35
                        );

                trackTieMeshTmp->addPoint(left, glm::vec3(1.0), glm::vec2(1.0), colorTrack);
                trackTieMeshTmp->addPoint(leftmiddle, glm::vec3(1.0), glm::vec2(1.0), colorTrack);

                trackTieMeshTmp->addPoint(leftmiddle, glm::vec3(1.0), glm::vec2(1.0), colorTrack);
                trackTieMeshTmp->addPoint(rightmiddle, glm::vec3(1.0), glm::vec2(1.0), colorTrack);

                trackTieMeshTmp->addPoint(rightmiddle, glm::vec3(1.0), glm::vec2(1.0), colorTrack);
                trackTieMeshTmp->addPoint(right, glm::vec3(1.0), glm::vec2(1.0), colorTrack);*/

                for(int t=0; t<pointsTrack.size(); t++) {
                    qvector3d currentPointBones;

                    for(int k=0; k < pointsTrack[t].size(); k++) {
                        currentPointBones.append(glm::vec3(
                            curSection->lNodes.at(j)->fPosHeartx(Track->fHeart+pointsTrack[t][k].y)+curSection->lNodes.at(j)->vLatHeart(Track->fHeart).x*pointsTrack[t][k].x,
                            curSection->lNodes.at(j)->fPosHearty(Track->fHeart+pointsTrack[t][k].y)+curSection->lNodes.at(j)->vLatHeart(Track->fHeart).y*pointsTrack[t][k].x,
                            curSection->lNodes.at(j)->fPosHeartz(Track->fHeart+pointsTrack[t][k].y)+curSection->lNodes.at(j)->vLatHeart(Track->fHeart).z*pointsTrack[t][k].x
                        ));
                    }

                    if(currentPointBones.size()>1 && lastPointBones[t].size()) for(int k=0; k < currentPointBones.size(); k++) {
                        int nextIndex = k==currentPointBones.size()-1?0:k+1;

                        glm::vec3 u = currentPointBones[nextIndex]-lastPointBones[t][k];
                        glm::vec3 v = currentPointBones[k]-lastPointBones[t][k];
                        glm::vec3 nms = glm::normalize(glm::cross(u,v));

                        trackBoneMeshTmp->addPoint(lastPointBones[t][k], nms, glm::vec2(1.0), colorTrack);
                        trackBoneMeshTmp->addPoint(currentPointBones[nextIndex], nms, glm::vec2(1.0), colorTrack);
                        trackBoneMeshTmp->addPoint(currentPointBones[k], nms, glm::vec2(1.0), colorTrack);

                        u = lastPointBones[t][nextIndex]-lastPointBones[t][k];
                        v = currentPointBones[nextIndex]-lastPointBones[t][k];
                        nms = glm::normalize(glm::cross(u,v));

                        trackBoneMeshTmp->addPoint(lastPointBones[t][k], nms, glm::vec2(1.0), colorTrack);
                        trackBoneMeshTmp->addPoint(lastPointBones[t][nextIndex], nms, glm::vec2(1.0), colorTrack);
                        trackBoneMeshTmp->addPoint(currentPointBones[nextIndex], nms, glm::vec2(1.0), colorTrack);
                    }

                    lastPointBones[t] = currentPointBones;
                }
            }
        }
    }

    distFromLastNode = 1.4f;
    int countTies = 0;
    for(int i = 0; i < Track->lSections.size(); i++) {
        section* curSection = Track->lSections[i];
        for(int j = 0; j < curSection->lNodes.size(); ++j) {
            distFromLastNode += curSection->lNodes.at(j)->fDistFromLast;
            if(distFromLastNode >= 1.4f) {
                if(distFromLastNode >= 1.4f) distFromLastNode-=1.4f;
                else distFromLastNode = 0.f;

                Mesh *tmpTieMesh;
                if(countTies == 8) {
                    tmpTieMesh = new Mesh(*currentSpineStyle->alternativeTie->mesh);
                    countTies = 0;
                } else {
                    tmpTieMesh = new Mesh(*currentSpineStyle->primaryTie->mesh);
                    countTies++;
                }

                mnode *t = curSection->lNodes.at(j);

                glm::mat3 rotation(t->vDirHeart(Track->fHeart), t->vNorm, -t->vLatHeart(Track->fHeart));
                glm::mat4 _m;
                _m = glm::translate(_m, t->fPosHeart(Track->fHeart));
                glm::mat4 rot = _m * glm::mat4(rotation);
                rot = glm::rotate(rot, 180.0f, glm::vec3(0, 0, 1));

                tmpTieMesh->applyMatrix(rot);
                trackTieMeshTmp->addVerts(tmpTieMesh);
                tmpTieMesh->clear();

                delete tmpTieMesh;
            }
        }
    }
}

void rendererTrackBuilder::render() {
    if(changes) {
        delete heartlineObject;
        delete heartlineMesh;

        heartlineMesh = new Mesh(*tmpHeartlineMesh);

        // Lined Track
        delete trackRailLeftMesh, trackRailRightMesh, trackRailBottomMesh, trackTieMesh;
        delete trackRailLeftObject, trackRailRightObject, trackRailBottomObject;

        delete trackBoneMesh, trackBoneMeshObject;
        trackBoneMesh = new Mesh(*trackBoneMeshTmp);

        delete trackRailMesh, trackRailMeshObject;
        trackRailMesh = new Mesh(*trackRailMeshTmp);

        trackRailLeftMesh = new Mesh(*trackRailLeftMeshTmp);
        trackRailRightMesh = new Mesh(*trackRailRightMeshTmp);
        trackRailBottomMesh = new Mesh(*trackRailBottomMeshTmp);
        trackTieMesh = new Mesh(*trackTieMeshTmp);

        glm::mat4 trm = glm::translate(trackBuilder->BuilderTrack->startPos) * glm::rotate(trackBuilder->BuilderTrack->startYaw, glm::vec3(0.f, 1.f, 0.f));
        trackRailLeftMesh->applyMatrix(trm);
        trackRailRightMesh->applyMatrix(trm);
        trackRailBottomMesh->applyMatrix(trm);
        trackTieMesh->applyMatrix(trm);
        heartlineMesh->applyMatrix(trm);
        trackBoneMesh->applyMatrix(trm);
        trackRailMesh->applyMatrix(trm);

        trackRailLeftObject = new VertexBufferObjectExtended();
        trackRailLeftObject->setData(trackRailLeftMesh);

        trackRailRightObject = new VertexBufferObjectExtended();
        trackRailRightObject->setData(trackRailRightMesh);

        trackRailBottomObject = new VertexBufferObjectExtended();
        trackRailBottomObject->setData(trackRailBottomMesh);

        heartlineObject = new VertexBufferObjectExtended();
        heartlineObject->setData(heartlineMesh);

        trackBoneMeshObject = new VertexBufferObjectExtended();
        trackBoneMeshObject->setData(trackBoneMesh);

        trackRailMeshObject = new VertexBufferObjectExtended();
        trackRailMeshObject->setData(trackRailMesh);

        trackTieMeshObject = new VertexBufferObjectExtended();
        trackTieMeshObject->setData(trackTieMesh);

        changes = false;
    }
    if(allowRender) {
        if(trackBuilder->showHeartline) heartlineObject->drawLine();

        //trackRailLeftObject->drawLine();
        //trackRailRightObject->drawLine();
        //trackRailBottomObject->drawLine();
        //trackTieObject->drawLine(GL_LINES);
        trackBoneMeshObject->drawVBO();
        trackRailMeshObject->drawVBO();
        trackTieMeshObject->drawVBO();
     }
}

void rendererTrackBuilder::update() {
    //allowRender = false;
    if(!changes) {
        //updateHeatValues();
        rebuildHeartlineAndTrack();
        changes = true;
        allowRender = true;
    }
    //allowRender = true;
}
