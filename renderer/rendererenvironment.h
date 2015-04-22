#ifndef RENDERERENVIRONMENT_H
#define RENDERERENVIRONMENT_H

#include <common/global.h>

#include <maingl/glwidget.h>
#include <renderer/renderer.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include <common/objloaderbuffer.h>

class GLWidget;

class QGLShaderProgram;
class QGLShader;

class rendererEnvironment : public renderer
{
public:
    rendererEnvironment(GLWidget& _glw);

protected:
    void render();
    void init();
    void update();
    void run();

private:
    GLWidget& glw;
    glm::vec2 size;

    Mesh *groundMesh;
    VertexBufferObject *vboObject;
    objLoaderBuffer *skyDomeSphere;

    // Textures
    GLuint textureGrass;
};

#endif // RENDERERENVIRONMENT_H
