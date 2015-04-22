#include "vertexbufferobjectextended.h"

VertexBufferObjectExtended::VertexBufferObjectExtended()
{
}


void VertexBufferObjectExtended::drawLine(GLuint drawMethod) {
    if(isset) {
#ifdef Q_OS_MAC
        glLineWidth(1.0f);

        glBindVertexArray(vao);
        glEnableVertexAttribArray(NoLimitsRenderer::currentShader->positionLoc);
        glEnableVertexAttribArray(NoLimitsRenderer::currentShader->normalLoc);
        glEnableVertexAttribArray(NoLimitsRenderer::currentShader->coordLoc);
        if(mesh->secondTexcoord.size()) {
            glEnableVertexAttribArray(NoLimitsRenderer::currentShader->coordSecondLoc);
        }
        glEnableVertexAttribArray(NoLimitsRenderer::currentShader->colorLoc);

        glDrawArrays(drawMethod, 0, mesh->verticies.size());

        glDisableVertexAttribArray(NoLimitsRenderer::currentShader->positionLoc);
        glDisableVertexAttribArray(NoLimitsRenderer::currentShader->normalLoc);
        glDisableVertexAttribArray(NoLimitsRenderer::currentShader->coordLoc);
        if(mesh->secondTexcoord.size()) {
            glDisableVertexAttribArray(NoLimitsRenderer::currentShader->coordSecondLoc);
        }
        glDisableVertexAttribArray(NoLimitsRenderer::currentShader->colorLoc);
        glBindVertexArray(0);
#endif
#ifdef Q_OS_WIN
    glBindVertexArray(vao);
    glDrawArrays(drawMethod, 0, mesh->verticies.size() );
    glBindVertexArray(0);
#endif
    }
}
