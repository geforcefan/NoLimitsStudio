#include <common/global.h>
#include <common/shader.h>
#include <QtCore>
#include <QGLWidget>

#include "textures.h"

textures::textures()
{
    GLuint texLoc = NoLimitsRenderer::currentShader->uniformLocation("firstTextureMap");
    glUniform1i(texLoc, 0);
    texLoc = NoLimitsRenderer::currentShader->uniformLocation("secondTextureMap");
    glUniform1i(texLoc, 1);

    texSamplerUse = NoLimitsRenderer::currentShader->uniformLocation("useTexSampler");
    secondTexSamplerUse = NoLimitsRenderer::currentShader->uniformLocation("useSecondTexSampler");
}

GLuint textures::getTexture(char *name) {
    return _textures[std::string(name)];
}

GLuint textures::loadTexture(char *name, char *_filename) {
    return this->loadTexture(name, _filename, true);
}

void textures::bindTexture(GLuint tex) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);

    glUniform1i(texSamplerUse, 1 );
}

void textures::bindSecondTexture(GLuint tex) {
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tex);

    glUniform1i(secondTexSamplerUse, 1 );
}

void textures::releaseTexture() {
    glBindTexture(GL_TEXTURE_2D , 0);
    glUniform1i(texSamplerUse, 0 );
}

void textures::releaseSecondTexture() {
    glBindTexture(GL_TEXTURE_2D , 0);
    glUniform1i(secondTexSamplerUse, 0 );
}

GLuint textures::loadTexture(char *name, char *_filename, bool fromResource) {
  QImage t;
  QImage b;

  QString filename;
  if(fromResource) filename = QString("").append(_filename);
  else filename = QString(_filename);

  if (!b.load(filename)) {
    qDebug("Loading Texture file: %s not found", _filename );
    b = QImage( 16, 16, QImage::Format_ARGB32 );
    b.fill( Qt::white );
  } else qDebug("Loading Texture file: %s", _filename );

  GLuint out;

  t = QGLWidget::convertToGLFormat( b );
  glGenTextures(1, &out);
  glBindTexture(GL_TEXTURE_2D, out);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, t.width(), t.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits());
  glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);
  _textures[std::string(name)] = out;

  return out;
}
