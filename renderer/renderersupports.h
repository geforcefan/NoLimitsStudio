#ifndef RENDERERSUPPORTS_H
#define RENDERERSUPPORTS_H

#include <common/global.h>
#include <renderer/renderer.h>

class GLWidget;
class objLoaderBuffer;
class VertexBufferObject;
class Mesh;
class rendererTrackStyles;
class rendererTrackStyle;

class rendererSupports : public renderer
{
public:
    rendererSupports(GLWidget& _glw);
protected:
    void render();
    void init();
    void update();
    void run();

private:
    glm::vec3 getSegmentOffset(int segmentIndex, int nodeIndex);

    GLWidget& glw;

    VertexBufferObject *supportObject;
    VertexBufferObject *supportCapObject;
    VertexBufferObject *connectorsObject;

    // Footer Sizes
    std::map<int, float> footerScales;
    std::map<int, vec4List> supportForms;

    Mesh *supportsMesh;
    Mesh *supportsCapMesh;
    Mesh *connectorsMesh;

    rendererTrackStyles *TrackStyles;
    rendererTrackStyle *TrackStyle;
};

#endif // RENDERERSUPPORTS_H
