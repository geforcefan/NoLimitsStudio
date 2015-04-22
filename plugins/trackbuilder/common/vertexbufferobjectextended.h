#ifndef VERTEXBUFFEROBJECTEXTENDED_H
#define VERTEXBUFFEROBJECTEXTENDED_H

#include <common/vertexbufferobject.h>
#include <common/global.h>
#include <common/Mesh.h>

#include <QGLBuffer>

class VertexBufferObjectExtended : public VertexBufferObject
{
public:
    VertexBufferObjectExtended();
    void drawLine(GLuint drawMethod = GL_LINE_STRIP);
};

#endif // VERTEXBUFFEROBJECTEXTENDED_H
