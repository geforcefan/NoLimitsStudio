#ifndef VBO_H
#define VBO_H

#include <common/global.h>
#include <common/Mesh.h>
#include <common/shader.h>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#ifdef Q_OS_WIN
#include <QGLBuffer>

class VertexBufferObject
{
public:
    VertexBufferObject();
    ~VertexBufferObject();
    void setData(Mesh* mesh);
    void drawVBO();

    GLuint vao;
    QGLBuffer *m_vertexBuffer;
    QGLBuffer *m_colorBuffer;
    QGLBuffer *m_normalBuffer;
    QGLBuffer *m_coordBuffer;
    QGLBuffer *m_2ndCoordBuffer;

    bool isset;

    Mesh* mesh;
};
#endif

#ifdef Q_OS_MAC
class VertexBufferObject
{
public:
    VertexBufferObject();
    ~VertexBufferObject();
    void setData(Mesh* mesh);
    void drawVBO();

    bool isset;
protected:
    GLuint vertexBuffer;
    GLuint normalBuffer;
    GLuint coordBuffer;
    GLuint coordSecondBuffer;
    GLuint indexBuffer;
    GLuint colorBuffer;

    Mesh* mesh;
    GLuint vao;
};
#endif

#endif
