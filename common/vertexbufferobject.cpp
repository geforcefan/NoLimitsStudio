#include <common/VertexBufferObject.h>

#include <QDebug>
#include <common/shader.h>

#include <iostream>

#include <maingl/glrenderer.h>
#include <maingl/glthread.h>
#include <maingl/glwidget.h>

VertexBufferObject::VertexBufferObject()
{
    isset = false;
}

#ifdef Q_OS_WIN

#include <QGLShader>

void VertexBufferObject::setData(Mesh* _mesh) {
    mesh = _mesh;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Position Buffer
    m_vertexBuffer = new QGLBuffer();
    m_vertexBuffer->create();
    m_vertexBuffer->setUsagePattern( QGLBuffer::DynamicDraw );
    if (!m_vertexBuffer->bind()) {
        qWarning() << "Could not bind vertex buffer to the context";
        return;
    }

    m_vertexBuffer->allocate( &mesh->verticies[0], mesh->verticies.size() * sizeof(glm::vec3) );
    NoLimitsRenderer::currentShader->ShaderProgram->setAttributeBuffer( "vertexPosition", GL_FLOAT, 0, 3 );
    NoLimitsRenderer::currentShader->ShaderProgram->enableAttributeArray( "vertexPosition" );
    m_vertexBuffer->release();

    // Color Buffer
    m_colorBuffer = new QGLBuffer();
    m_colorBuffer->create();
    m_colorBuffer->setUsagePattern( QGLBuffer::DynamicDraw );
    if (!m_colorBuffer->bind()) {
        qWarning() << "Could not bind color buffer to the context";
        return;
    }

    m_colorBuffer->allocate( &mesh->colors[0], mesh->colors.size() * sizeof(glm::vec4) );
    NoLimitsRenderer::currentShader->ShaderProgram->setAttributeBuffer( "vertexColor", GL_FLOAT, 0, 4 );
    NoLimitsRenderer::currentShader->ShaderProgram->enableAttributeArray( "vertexColor" );
    m_colorBuffer->release();

    // Normal Buffer
    m_normalBuffer = new QGLBuffer();
    m_normalBuffer->create();
    m_normalBuffer->setUsagePattern( QGLBuffer::DynamicDraw );
    if (!m_normalBuffer->bind()) {
        qWarning() << "Could not bind normal buffer to the context";
        return;
    }

    m_normalBuffer->allocate( &mesh->normals[0], mesh->normals.size() * sizeof(glm::vec3) );
    NoLimitsRenderer::currentShader->ShaderProgram->setAttributeBuffer( "vertexNormal", GL_FLOAT, 0, 3 );
    NoLimitsRenderer::currentShader->ShaderProgram->enableAttributeArray( "vertexNormal" );
    m_normalBuffer->release();

    // Text Coord Buffer
    m_coordBuffer = new QGLBuffer();
    m_coordBuffer->create();
    m_coordBuffer->setUsagePattern( QGLBuffer::DynamicDraw );
    if (!m_coordBuffer->bind()) {
        qWarning() << "Could not bind coord buffer to the context";
        return;
    }

    m_coordBuffer->allocate( &mesh->texcoord[0], mesh->texcoord.size() * sizeof(glm::vec2) );
    NoLimitsRenderer::currentShader->ShaderProgram->setAttributeBuffer( "vertexCoord", GL_FLOAT, 0, 2 );
    NoLimitsRenderer::currentShader->ShaderProgram->enableAttributeArray( "vertexCoord" );
    m_coordBuffer->release();


    if(mesh->secondTexcoord.size()) {
        // 2nd Text Coord Buffer
        m_2ndCoordBuffer = new QGLBuffer();
        m_2ndCoordBuffer->create();
        m_2ndCoordBuffer->setUsagePattern( QGLBuffer::DynamicDraw );
        if (!m_2ndCoordBuffer->bind()) {
            qWarning() << "Could not bind 2ndCoordBuffer buffer to the context";
            return;
        }

        m_2ndCoordBuffer->allocate( &mesh->secondTexcoord[0], mesh->secondTexcoord.size() * sizeof(glm::vec2) );
        NoLimitsRenderer::currentShader->ShaderProgram->setAttributeBuffer( "vertexSecondCoord", GL_FLOAT, 0, 2 );
        NoLimitsRenderer::currentShader->ShaderProgram->enableAttributeArray( "vertexSecondCoord" );
        m_2ndCoordBuffer->release();
    }
    glBindVertexArray(0);

    isset=true;
}

void VertexBufferObject::drawVBO()
{
    if(isset) {
        glBindVertexArray(vao);
        glDrawArrays( GL_TRIANGLES, 0, mesh->verticies.size() );
        glBindVertexArray(0);
    }

}
#endif

#ifdef Q_OS_MAC
void VertexBufferObject::setData(Mesh* _mesh) {
    isset=true;
    mesh = _mesh;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, mesh->verticies.size() * sizeof(float), &mesh->verticies[0], GL_STATIC_DRAW);

    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, mesh->normals.size() * sizeof(float), &mesh->normals[0], GL_STATIC_DRAW);

    glGenBuffers(1, &coordBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, coordBuffer);
    glBufferData(GL_ARRAY_BUFFER, mesh->texcoord.size() * sizeof(float), &mesh->texcoord[0], GL_STATIC_DRAW);

    if(mesh->secondTexcoord.size()) {
        glGenBuffers(1, &coordSecondBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, coordSecondBuffer);
        glBufferData(GL_ARRAY_BUFFER, mesh->secondTexcoord.size() * sizeof(float), &mesh->secondTexcoord[0], GL_STATIC_DRAW);
    }

    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, mesh->colors.size() * sizeof(float), &mesh->colors[0], GL_STATIC_DRAW);

    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indicies.size() * sizeof(unsigned short), &mesh->indicies[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(NoLimitsRenderer::currentShader->positionLoc);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(
        NoLimitsRenderer::currentShader->positionLoc,        // attribute. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    glEnableVertexAttribArray(NoLimitsRenderer::currentShader->normalLoc);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glVertexAttribPointer(
        NoLimitsRenderer::currentShader->normalLoc,           // attribute. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    glEnableVertexAttribArray(NoLimitsRenderer::currentShader->coordLoc);
    glBindBuffer(GL_ARRAY_BUFFER, coordBuffer);
    glVertexAttribPointer(
        NoLimitsRenderer::currentShader->coordLoc,           // attribute. No particular reason for 0, but must match the layout in the shader.
        2,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    if(mesh->secondTexcoord.size()) {
        glEnableVertexAttribArray(NoLimitsRenderer::currentShader->coordSecondLoc);
        glBindBuffer(GL_ARRAY_BUFFER, coordSecondBuffer);
        glVertexAttribPointer(
            NoLimitsRenderer::currentShader->coordSecondLoc,           // attribute. No particular reason for 0, but must match the layout in the shader.
            2,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
        );
    }

    glEnableVertexAttribArray(NoLimitsRenderer::currentShader->colorLoc);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glVertexAttribPointer(
        NoLimitsRenderer::currentShader->colorLoc,           // attribute. No particular reason for 0, but must match the layout in the shader.
        4,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    glBindVertexArray(0);
}

void VertexBufferObject::drawVBO()
{
    if(isset) {
        glBindVertexArray(vao);
        glEnableVertexAttribArray(NoLimitsRenderer::currentShader->positionLoc);
        glEnableVertexAttribArray(NoLimitsRenderer::currentShader->normalLoc);
        glEnableVertexAttribArray(NoLimitsRenderer::currentShader->coordLoc);
        if(mesh->secondTexcoord.size()) {
            glEnableVertexAttribArray(NoLimitsRenderer::currentShader->coordSecondLoc);
        }
        glEnableVertexAttribArray(NoLimitsRenderer::currentShader->colorLoc);

        glDrawArrays(GL_TRIANGLES, 0, mesh->verticies.size());

        glDisableVertexAttribArray(NoLimitsRenderer::currentShader->positionLoc);
        glDisableVertexAttribArray(NoLimitsRenderer::currentShader->normalLoc);
        glDisableVertexAttribArray(NoLimitsRenderer::currentShader->coordLoc);
        if(mesh->secondTexcoord.size()) {
            glDisableVertexAttribArray(NoLimitsRenderer::currentShader->coordSecondLoc);
        }
        glDisableVertexAttribArray(NoLimitsRenderer::currentShader->colorLoc);
        glBindVertexArray(0);
    }
}
#endif

VertexBufferObject::~VertexBufferObject()
{
    /*
#ifdef Q_OS_WIN
    if(isset) {
        m_vertexBuffer->destroy();
        m_vertexBuffer->destroy();
        m_normalBuffer->destroy();
        m_coordBuffer->destroy();

        delete m_vertexBuffer;
        delete m_colorBuffer;
        delete m_normalBuffer;
        delete m_coordBuffer;

        if(mesh->secondTexcoord.size()) {
            m_2ndCoordBuffer->destroy();
            delete m_2ndCoordBuffer;
        }
    }
#endif
*/
}
