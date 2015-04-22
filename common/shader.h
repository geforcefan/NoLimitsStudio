#ifndef SHADER_H
#define SHADER_H

#include <common/global.h>

#include <iostream>

#ifdef Q_OS_WIN
class QGLShaderProgram;
class QGLShader;

class shader
{
public:
    shader();
    ~shader();
    shader(QString vshader, QString fshader);

    void loadShader(QString vshader, QString fshader);
    void bind();
    void release();
    GLuint uniformLocation(char *name);
    GLuint attrLocation(char *name);

    QGLShaderProgram *ShaderProgram;
    QGLShader *VertexShader, *FragmentShader;
};
#endif

#ifdef Q_OS_MAC
class shader
{
public:
    shader();
    ~shader();
    shader(QString vshader, QString fshader);

    void loadShader(QString vshader, QString fshader);
    void bind();
    void release();
    GLuint uniformLocation(char *name);
    GLuint attrLocation(char *name);

    GLuint positionLoc;
    GLuint normalLoc;
    GLuint coordLoc;
    GLuint coordSecondLoc;
    GLuint colorLoc;
private:
    GLuint programObject;
    GLuint vertexShader;
    GLuint fragmentShader;

    std::map<std::string, GLuint> _unifromBuffer;
};
#endif

#endif // SHADER_H
