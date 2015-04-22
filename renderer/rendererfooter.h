#ifndef RENDERERFOOTER_H
#define RENDERERFOOTER_H

#include <common/global.h>
#include <renderer/renderer.h>

class GLWidget;
class objLoaderBuffer;
class VertexBufferObject;
class Mesh;

class rendererFooter : public renderer
{
public:
    rendererFooter(GLWidget& _glw);
protected:
    void render();
    void init();
    void update();
    void run();

private:
    GLWidget& glw;

    VertexBufferObject *baseObject, *plateTopObject, *plateBottomObject, *screwsObject;
    Mesh *baseObjectMesh, *plateTopObjectMesh, *plateBottomObjectMesh, *screwsObjectMesh;

    // Textures
    GLuint textureBase;

    // Footer Sizes
    std::map<int, float> footerScales;
    std::map<int, glm::vec3> footerPosition;

    // OBJ Files

    objLoaderBuffer *_baseObject;
    objLoaderBuffer *_plateTopObject;
    objLoaderBuffer *_plateBottomObject;
    objLoaderBuffer *_screwsObject;
};

#endif // RENDERERFOOTER_H
