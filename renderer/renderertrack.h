#ifndef RENDERERTRACK_H
#define RENDERERTRACK_H

#include <common/global.h>

#include <maingl/glwidget.h>
#include <renderer/renderer.h>

#include <vector>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <common/extrude.h>
#include <common/objloader.hpp>
#include <common/VertexBufferObject.h>
#include <common/objloaderbuffer.h>
#include <vector>

#include <common/nlbanking.h>

class renderer;
class rendererTrackStyles;
class rendererTrackThreadTies;
class rendererTrackStyleSpineStyle;
class vectorStuff;

typedef QVector<glm::vec2> qvector2d;
typedef QVector<glm::vec3> qvector3d;

// Main Renderer
class rendererTrack : public renderer
{
public:
    rendererTrack(NlTrack *inputTrack, GLWidget& _glw);

    void render();
    void init();
    void update();
    void run();

    int currentSegment;
    rendererTrackStyleSpineStyle *currentSpineStyle;

    // Segments List
    std::vector<int> segmentsListStart;
    std::vector<int> segmentsListEnd;
    std::vector<int> segmentsListSpineStyle;

    float startOffset;
    float lastOffset;
    int countTies;

    vectorStuff *VectorStuffTies;

    vec4List getTrackPoints(float division, int fromSegment, int toSegment);
    vec4List getTrackPoints(float division, int fromSegment, int toSegment, float startOffset, float &lastOffset);
    vec4List getTrackPoints(float division, int fromSegment, int toSegment, bool completeEnd, float startOffset, float &lastOffset);

    bool firstInit;

    void generateTrack();

    GLWidget& glw;

    rendererTrackStyles* TrackStyles;

    // Track meshes
    VertexBufferObject *railObject;
    VertexBufferObject *spineObject;
    VertexBufferObject *tieObject;

    // Meshes
    Mesh *railMesh;
    Mesh *spineMesh;
    Mesh *tieMesh;

    std::vector<int> points2segments;
    std::vector<float> points2t;

    //
    std::vector<bankingValues> tmpBankingValues;

    // Threads
    rendererTrackThreadTies *threadTies;

    bool useRelative;

    GLuint textTex;

    NlTrack* Track;
};
#endif // RENDERERTRACK_H
