#ifndef RENDERERTERRAIN_H
#define RENDERERTERRAIN_H

#include <common/global.h>
#include <renderer/renderer.h>

class GLWidget;
class objLoaderBuffer;
class VertexBufferObject;
class Mesh;

class rendererTerrain : public renderer
{
public:
    rendererTerrain(GLWidget& _glw);
protected:
    void render();
    void init();
    void update();
    void run();

    VertexBufferObject *terrainObject;
    Mesh *terrainMesh;

    VertexBufferObject *waterObject;
    Mesh *waterMesh;
private:
    GLWidget& glw;

    GLuint terrainTexture;
    GLuint terrainDetailTexture;
    GLuint waterTexture;
};

#endif // RENDERERTERRAIN_H
