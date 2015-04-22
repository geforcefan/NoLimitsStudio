#include <common/global.h>

#include "rendererTrack.h"
#include <common/nlsegment.h>
#include <common/vectorstuff.h>
#include <common/shader.h>
#include <common/textures.h>
#include <common/nlbanking.h>
#include <common/settings.h>
#include <renderer/renderertrack/renderertrackstyles.h>

#include <maingl/glrenderer.h>

#include <iostream>
#include <fstream>
#include <istream>
#include <sstream>

#include <QDebug>

#include <ctime>

rendererTrack::rendererTrack(NlTrack* inputTrack, GLWidget& _glw) : glw(_glw) {
    renderer();

    Track = inputTrack;

    railObject = new VertexBufferObject();
    spineObject = new VertexBufferObject();
    tieObject = new VertexBufferObject();

    railMesh = new Mesh();
    spineMesh = new Mesh();
    tieMesh = new Mesh();

    useRelative = false;
}

void rendererTrack::init() {
    TrackStyles = new rendererTrackStyles();

    textTex = NoLimitsRenderer::Textures->loadTexture("rendererEnvironment:grass", "/Users/ercanakyurek/Daten/glTest/textures/Untitled-2.png");

    //this->glw.getGLThread().init();
}

vec4List rendererTrack::getTrackPoints(float division, int fromSegment, int toSegment) {
    float lastOffset = 0.0f;
    return getTrackPoints(division, fromSegment, toSegment, true, 0.0f, lastOffset);
}

vec4List rendererTrack::getTrackPoints(float division, int fromSegment, int toSegment, float startOffset, float &lastOffset) {
    return getTrackPoints(division, fromSegment, toSegment, true, startOffset, lastOffset);
}

vec4List rendererTrack::getTrackPoints(float division, int fromSegment, int toSegment, bool completeEnd, float startOffset, float &lastOffset) {
    vec4List TrackPoints;
    tmpBankingValues.clear();
    if(!Track->totalSegments) return TrackPoints;

    float lastStartOffset = startOffset;
    glm::vec3 p1;
    NLSegment *segmentUtil = new NLSegment(0);

    points2segments.clear();
    points2t.clear();

    float length, tiePosition, numTies, t, banking;

    for(int i=fromSegment; i < toSegment+1; i++) {
        delete segmentUtil;
        segmentUtil = new NLSegment(i);

        length = segmentUtil->getLength();
        numTies = ceil((length-lastStartOffset)/division);
        tiePosition = 0.0f;

        for(int offset=0; offset < numTies; offset++) {
            tiePosition = lastStartOffset + (offset*division);

            float t = tiePosition/length;

            if(useRelative || length < 2.0f) {
                p1 = segmentUtil->getRelativePosition(t);
            } else {
                p1 = segmentUtil->getOffsetPosition(tiePosition);
                t = segmentUtil->lastOffset;
            }
            banking = segmentUtil->getBanking(t) * (180 / M_PI);
            bankingValues dir = segmentUtil->getDir(t);

            tmpBankingValues.push_back(dir);
            TrackPoints.push_back(glm::vec4(p1, banking));
            points2segments.push_back(i);
            points2t.push_back(t);
        }

        lastStartOffset = division-(length-tiePosition);
    }
    lastOffset = lastStartOffset;

    banking = segmentUtil->getBanking(1.0f) * (180 / M_PI);
    if(completeEnd) TrackPoints.push_back(glm::vec4(segmentUtil->getRelativePosition(1.0f), banking));
    points2segments.push_back(toSegment);

    delete segmentUtil;

    return TrackPoints;
}

void rendererTrack::update() {
    allowRender = false;

    vectorStuff *VectorStuffTies = new vectorStuff();
    VectorStuffTies->bankingVector = glm::vec3(1,0,0);
    VectorStuffTies->rotationVector = glm::vec3(0,0,-1);

    delete railObject, spineObject, tieObject;
    railObject = new VertexBufferObject();
    spineObject = new VertexBufferObject();
    tieObject = new VertexBufferObject();

    // Meshes
    delete railMesh, spineMesh, tieMesh;
    railMesh = new Mesh();
    spineMesh = new Mesh();
    tieMesh = new Mesh();

    rendererTrackStyle *TrackStyle;
    TrackStyle = TrackStyles->Styles[Track->style];
    if(TrackStyle==0) TrackStyle  = TrackStyles->Styles[CLASSIC_STEEL_LOOPING];

    bool closedTrack = Track->trackClosedCircuit?true:false;

    // Main Rails are always the same
    useRelative = true;
    vec4List mainRailsList = getTrackPoints(TrackStyle->basicRailSpacing, 0, Track->totalSegments-1);

    vec4List _maincolors;
    if( Track->totalSegments) for(int i = 0; i < points2segments.size(); i++) {
        SEGMSegment segment = Track->segs[points2segments[i]];
        glm::vec4 railColor = segment.individualTrackColor?NoLimitsRenderer::makeColor(
           segment.trackRailColor.r,
           segment.trackRailColor.g,
           segment.trackRailColor.b,
           segment.trackRailColor.a
           ):NoLimitsRenderer::makeColor(
           Track->trackRailColor.r,
           Track->trackRailColor.g,
           Track->trackRailColor.b,
           Track->trackRailColor.a
           );
        _maincolors.push_back(railColor);
    }

    std::map<int, qvector3d> lastPointRail;

    for(int j = 0; j < mainRailsList.size(); j++) {
        int firstIndex = (j==mainRailsList.size()-1)?j-1:j;
        glm::vec4 pos = mainRailsList[firstIndex];
        glm::vec3 normal = tmpBankingValues[firstIndex].normal;
        glm::vec3 binormal = tmpBankingValues[firstIndex].binormal;
        glm::vec4 color = _maincolors[firstIndex];

        for(int t=0; t<TrackStyle->basicRails.size(); t++) {
            qvector3d currentPointRail;

            for(int k=TrackStyle->basicRails[t].size()-1; k >= 0; k--) {
                currentPointRail.append(glm::vec3(
                    (pos.x + (normal.x * TrackStyle->basicRails[t][k].x))+((binormal.x * TrackStyle->basicRails[t][k].y)),
                    (pos.y + (normal.y * TrackStyle->basicRails[t][k].x))+((binormal.y * TrackStyle->basicRails[t][k].y)),
                    (pos.z + (normal.z * TrackStyle->basicRails[t][k].x))+((binormal.z * TrackStyle->basicRails[t][k].y))
                ));
            }

            if(currentPointRail.size()>1 && lastPointRail[t].size()) for(int k=0; k < currentPointRail.size(); k++) {
                int nextIndex = k==currentPointRail.size()-1?0:k+1;

                glm::vec3 u = currentPointRail[nextIndex]-lastPointRail[t][k];
                glm::vec3 v = currentPointRail[k]-lastPointRail[t][k];
                glm::vec3 nms = glm::normalize(glm::cross(u,v));

                railMesh->addPoint(lastPointRail[t][k], nms, glm::vec2(1.0), color);
                railMesh->addPoint(currentPointRail[nextIndex], nms, glm::vec2(1.0), color);
                railMesh->addPoint(currentPointRail[k], nms, glm::vec2(1.0), color);

                u = lastPointRail[t][nextIndex]-lastPointRail[t][k];
                v = currentPointRail[nextIndex]-lastPointRail[t][k];
                nms = glm::normalize(glm::cross(u,v));

                railMesh->addPoint(lastPointRail[t][k], nms, glm::vec2(1.0), color);
                railMesh->addPoint(lastPointRail[t][nextIndex], nms, glm::vec2(1.0), color);
                railMesh->addPoint(currentPointRail[nextIndex], nms, glm::vec2(1.0), color);
            }

            lastPointRail[t] = currentPointRail;
        }

    }


    // Different Spine Styles
    int lastSpineStyle;
    int startSegment = 0;
    int endSegment = 0;

    useRelative = false;

    std::vector<int> segmentsListStart;
    std::vector<int> segmentsListEnd;
    std::vector<int> segmentsListSpineStyle;

    for(int i=0; i < Track->totalSegments; i++) {
        int currentSpineStyle = 0;
        currentSpineStyle = (Track->segs[i].flags & (1 << 2)?2:currentSpineStyle);
        currentSpineStyle = (Track->segs[i].flags & (1 << 3)?3:currentSpineStyle);
        currentSpineStyle = (!currentSpineStyle?1:currentSpineStyle);

        if(i && currentSpineStyle != lastSpineStyle) {
            endSegment = i-1;

            segmentsListStart.push_back(startSegment);
            segmentsListEnd.push_back(endSegment);
            segmentsListSpineStyle.push_back(lastSpineStyle);

            startSegment = i;
        }

        lastSpineStyle = currentSpineStyle;
    }
    if(segmentsListStart.size()) segmentsListStart.push_back(endSegment+1);
    else segmentsListStart.push_back(endSegment);

    segmentsListEnd.push_back(Track->totalSegments);
    segmentsListSpineStyle.push_back(lastSpineStyle);

    float startOffset = 0.0f;
    float lastOffset = 0.0f;
    int countTies = 1;

    for(int i=0; i < segmentsListStart.size(); i++) {
        rendererTrackStyleSpineStyle *currentSpineStyle = new rendererTrackStyleSpineStyle();
        currentSpineStyle = segmentsListSpineStyle[i] == 1?(TrackStyle->Spine0):(segmentsListSpineStyle[i] == 2?(TrackStyle->Spine1):(TrackStyle->Spine2));
        if(Track->totalSegments == segmentsListEnd[i]) segmentsListEnd[i]--;

        if(!currentSpineStyle) continue;


        // RAILS
        if(currentSpineStyle->rails.size()) {
            vec4List railList = getTrackPoints(currentSpineStyle->railSpacing, segmentsListStart[i], segmentsListEnd[i]);
            vec4List _colors;
            for(int i = 0; i < points2segments.size(); i++) {
                SEGMSegment segment = Track->segs[points2segments[i]];
                glm::vec4 railColor = segment.individualTrackColor?NoLimitsRenderer::makeColor(
                   segment.trackRailColor.r,
                   segment.trackRailColor.g,
                   segment.trackRailColor.b,
                   segment.trackRailColor.a
                   ):NoLimitsRenderer::makeColor(
                   Track->trackRailColor.r,
                   Track->trackRailColor.g,
                   Track->trackRailColor.b,
                   Track->trackRailColor.a
                   );
                _colors.push_back(railColor);
            }

            std::map<int, qvector3d> lastRailSpine;

            for(int j = 0; j < railList.size(); j++) {
                int firstIndex = (j==railList.size()-1)?j-1:j;
                glm::vec4 pos = railList[firstIndex];
                glm::vec3 normal = tmpBankingValues[firstIndex].normal;
                glm::vec3 binormal = tmpBankingValues[firstIndex].binormal;
                glm::vec4 color = _colors[firstIndex];

                for(int t=0; t<currentSpineStyle->rails.size(); t++) {
                    qvector3d currentPointRail;

                    for(int k=currentSpineStyle->rails[t].size()-1; k >= 0; k--) {
                        currentPointRail.append(glm::vec3(
                            (pos.x + (normal.x * -currentSpineStyle->rails[t][k].x))+((binormal.x * -currentSpineStyle->rails[t][k].y)),
                            (pos.y + (normal.y * -currentSpineStyle->rails[t][k].x))+((binormal.y * -currentSpineStyle->rails[t][k].y)),
                            (pos.z + (normal.z * -currentSpineStyle->rails[t][k].x))+((binormal.z * -currentSpineStyle->rails[t][k].y))
                        ));
                    }

                    if(currentPointRail.size()>1 && lastRailSpine[t].size()) for(int k=0; k < currentPointRail.size(); k++) {
                        int nextIndex = k==currentPointRail.size()-1?0:k+1;

                        glm::vec3 u = currentPointRail[nextIndex]-lastRailSpine[t][k];
                        glm::vec3 v = currentPointRail[k]-lastRailSpine[t][k];
                        glm::vec3 nms = glm::normalize(glm::cross(u,v));

                        railMesh->addPoint(lastRailSpine[t][k], nms, glm::vec2(1.0), color);
                        railMesh->addPoint(currentPointRail[nextIndex], nms, glm::vec2(1.0), color);
                        railMesh->addPoint(currentPointRail[k], nms, glm::vec2(1.0), color);

                        u = lastRailSpine[t][nextIndex]-lastRailSpine[t][k];
                        v = currentPointRail[nextIndex]-lastRailSpine[t][k];
                        nms = glm::normalize(glm::cross(u,v));

                        railMesh->addPoint(lastRailSpine[t][k], nms, glm::vec2(1.0), color);
                        railMesh->addPoint(lastRailSpine[t][nextIndex], nms, glm::vec2(1.0), color);
                        railMesh->addPoint(currentPointRail[nextIndex], nms, glm::vec2(1.0), color);
                    }

                    lastRailSpine[t] = currentPointRail;
                }
            }
        }

        // SPINE
        if(currentSpineStyle->spines.size()) {
            vec4List _colors;
            vec4List spinesList = getTrackPoints(currentSpineStyle->spineSpacing, segmentsListStart[i], segmentsListEnd[i]);
            if( Track->totalSegments) for(int i = 0; i < points2segments.size(); i++) {
                SEGMSegment segment = Track->segs[points2segments[i]];
                glm::vec4 spineColor = segment.individualTrackColor?NoLimitsRenderer::makeColor(
                   segment.trackSpineColor.r,
                   segment.trackSpineColor.g,
                   segment.trackSpineColor.b,
                   segment.trackSpineColor.a
                   ):NoLimitsRenderer::makeColor(
                   Track->trackSpineColor.r,
                   Track->trackSpineColor.g,
                   Track->trackSpineColor.b,
                   Track->trackSpineColor.a
                   );
                _colors.push_back(spineColor);
            }

            std::map<int, qvector3d> lastPointSpine;

            for(int j = 0; j < spinesList.size(); j++) {
                int firstIndex = (j==spinesList.size()-1)?j-1:j;
                glm::vec4 pos = spinesList[firstIndex];
                glm::vec3 normal = tmpBankingValues[firstIndex].normal;
                glm::vec3 binormal = tmpBankingValues[firstIndex].binormal;
                glm::vec4 color = _colors[firstIndex];

                for(int t=0; t<currentSpineStyle->spines.size(); t++) {
                    qvector3d currentPointSpine;

                    for(int k=currentSpineStyle->spines[t].size()-1; k >= 0; k--) {
                        currentPointSpine.append(glm::vec3(
                            (pos.x + (normal.x * -currentSpineStyle->spines[t][k].x))+((binormal.x * -currentSpineStyle->spines[t][k].y)),
                            (pos.y + (normal.y * -currentSpineStyle->spines[t][k].x))+((binormal.y * -currentSpineStyle->spines[t][k].y)),
                            (pos.z + (normal.z * -currentSpineStyle->spines[t][k].x))+((binormal.z * -currentSpineStyle->spines[t][k].y))
                        ));
                    }

                    if(currentPointSpine.size()>1 && lastPointSpine[t].size()) for(int k=0; k < currentPointSpine.size(); k++) {
                        int nextIndex = k==currentPointSpine.size()-1?0:k+1;

                        glm::vec3 u = currentPointSpine[nextIndex]-lastPointSpine[t][k];
                        glm::vec3 v = currentPointSpine[k]-lastPointSpine[t][k];
                        glm::vec3 nms = glm::normalize(glm::cross(u,v));

                        spineMesh->addPoint(lastPointSpine[t][k], nms, glm::vec2(1.0), color);
                        spineMesh->addPoint(currentPointSpine[nextIndex], nms, glm::vec2(1.0), color);
                        spineMesh->addPoint(currentPointSpine[k], nms, glm::vec2(1.0), color);

                        u = lastPointSpine[t][nextIndex]-lastPointSpine[t][k];
                        v = currentPointSpine[nextIndex]-lastPointSpine[t][k];
                        nms = glm::normalize(glm::cross(u,v));

                        spineMesh->addPoint(lastPointSpine[t][k], nms, glm::vec2(1.0), color);
                        spineMesh->addPoint(lastPointSpine[t][nextIndex], nms, glm::vec2(1.0), color);
                        spineMesh->addPoint(currentPointSpine[nextIndex], nms, glm::vec2(1.0), color);
                    }

                    lastPointSpine[t] = currentPointSpine;
                }
            }
        }


        //useRelative = false;
        // Tie generation for each spine styles change
        vec4List tieList = getTrackPoints(currentSpineStyle->tieSpacing, segmentsListStart[i], segmentsListEnd[i], startOffset, lastOffset);
        startOffset = lastOffset;
        if(tieList.size()) for(int k=0; k < tieList.size()-1; k++) {
            // Tie Color
            int segmentIndex = points2segments[k];
            NLSegment *seg = new NLSegment(segmentIndex);

            int firstIndex = (k==tieList.size()-1)?k-1:k;

            Mesh *tmpTieMesh;

            if((tieList.size()-3 < k) && currentSpineStyle->useLastTie) tmpTieMesh = new Mesh(*currentSpineStyle->lastTie->mesh);
            else if(currentSpineStyle->alternativeTieRepeat && countTies == currentSpineStyle->alternativeTieRepeat) {
                tmpTieMesh = new Mesh(*currentSpineStyle->alternativeTie->mesh);
                countTies = 1;
            } else tmpTieMesh = new Mesh(*currentSpineStyle->primaryTie->mesh);

            if(k < tieList.size()-2 && currentSpineStyle->useLastTie) {
                float tfrom = points2t[k];
                float tto = points2t[k+1];
                if(tto < tfrom) tto = 1.0f;
                bool hasRasc = false;

                for(int rascs = 0; rascs < Track->seg[segmentIndex].totalRASCs; rascs++) {
                    if(Track->seg[segmentIndex].data[rascs].type != RASC_TRACK_NODE) continue;
                    float t = seg->getTimeFromOffset(Track->seg[segmentIndex].data[rascs].pos);

                    if(t > tfrom && t < tto) hasRasc = true;
                }
                if(hasRasc) tmpTieMesh = new Mesh(*currentSpineStyle->lastTie->mesh);
            }

            glm::mat3 rotation(tmpBankingValues[firstIndex].tangent, tmpBankingValues[firstIndex].normal, tmpBankingValues[firstIndex].binormal);
            glm::mat4 _m;
            _m = glm::translate(_m, glm::vec3(tieList[firstIndex].x, tieList[firstIndex].y, tieList[firstIndex].z));
            glm::mat4 rot = _m * glm::mat4(rotation);
            rot = glm::rotate(rot, 180.0f, glm::vec3(0, 0, 1));

            tmpTieMesh->applyMatrix(rot);

            //tmpBankingValues

            glm::vec4 tieColor = Track->segs[segmentIndex].individualTrackColor?NoLimitsRenderer::makeColor(
               Track->segs[segmentIndex].trackCrosstieColor.r,
               Track->segs[segmentIndex].trackCrosstieColor.g,
               Track->segs[segmentIndex].trackCrosstieColor.b,
               Track->segs[segmentIndex].trackCrosstieColor.a
               ):NoLimitsRenderer::makeColor(
               Track->trackCrosstieColor.r,
               Track->trackCrosstieColor.g,
               Track->trackCrosstieColor.b,
               Track->trackCrosstieColor.a
            );
            tmpTieMesh->setColor(tieColor);

            tieMesh->addVerts(tmpTieMesh);
            delete tmpTieMesh;
            countTies++;
        }
    }

    delete VectorStuffTies;

    changes = true;
    allowRender = true;
}

void rendererTrack::render() {
    if(changes) {
        railObject->setData(railMesh);
        spineObject->setData(spineMesh);
        tieObject->setData(tieMesh);

        changes = false;
    }
    if(allowRender && NoLimitsRenderer::Settings->rendererTrack) {
        spineObject->drawVBO();
        railObject->drawVBO();
        tieObject->drawVBO();
    }
}
