#ifndef RENDERERTRACKBUILDER_H
#define RENDERERTRACKBUILDER_H

#include <common/global.h>
#include <renderer/renderer.h>

#include <map>
#include <renderer/renderertrack/renderertrackstyles.h>

#include <QMutex>

class GLWidget;
class objLoaderBuffer;
class VertexBufferObject;
class Mesh;
class vectorStuff;
class VertexBufferObjectExtended;

typedef QVector<glm::vec2> qvector2d;
typedef QVector<glm::vec3> qvector3d;

class rendererTrackBuilder : public renderer
{
public:
    rendererTrackBuilder();

    Mesh *heartlineMesh, *tmpHeartlineMesh;
    VertexBufferObjectExtended *heartlineObject;

    Mesh* trackRailLeftMesh;
    Mesh* trackRailRightMesh;
    Mesh* trackRailBottomMesh;
    Mesh* trackRailLeftMeshTmp;
    Mesh* trackRailRightMeshTmp;
    Mesh* trackRailBottomMeshTmp;
    VertexBufferObjectExtended* trackRailLeftObject;
    VertexBufferObjectExtended* trackRailRightObject;
    VertexBufferObjectExtended* trackRailBottomObject;

    QVector<qvector2d> pointsTrack;
    QVector<qvector2d> pointsRail;

    Mesh *trackBoneMeshTmp;
    Mesh *trackBoneMesh;
    VertexBufferObjectExtended* trackBoneMeshObject;

    Mesh *trackRailMeshTmp;
    Mesh *trackRailMesh;
    VertexBufferObjectExtended* trackRailMeshObject;


    Mesh *trackTieMeshTmp;
    Mesh *trackTieMesh;
    VertexBufferObjectExtended* trackTieMeshObject;

    rendererTrackStyle *TrackStyle;
    rendererTrackStyleSpineStyle *currentSpineStyle;

    QMutex *Mutex;
public:
    void render();
    void init();
    void update();
    void run();
    void updateHeatValues();
    void rebuildHeartlineAndTrack();

    float heatMinValue;
    float heatMaxValue;
    float heatMinVisibleWaveLength;
    float heatMaxVisibleWaveLength;
    bool useHeat;
};

#endif // RENDERERTRACKBUILDER_H
