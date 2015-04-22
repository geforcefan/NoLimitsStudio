#include <common/global.h>
#include "shader.h"

#ifdef Q_OS_WIN
#include <QGLShader>
#include <QFile>
#include <QFileInfo>
#endif

#include <iostream>

#include <maingl/glrenderer.h>
#include <maingl/glthread.h>
#include <maingl/glwidget.h>

#ifdef Q_OS_WIN
shader::shader() {
}

shader::shader(QString vshader, QString fshader) {
    loadShader(vshader, fshader);
}

void shader::loadShader(QString vshader, QString fshader) {
    ShaderProgram = new QGLShaderProgram;

    // load and compile vertex shader
    QFileInfo vsh(vshader);
    if(vsh.exists())
        {
        VertexShader = new QGLShader(QGLShader::Vertex);
        if(VertexShader->compileSourceFile(vshader))
            ShaderProgram->addShader(VertexShader);
        else qWarning() << "Vertex Shader Error" << VertexShader->log();
        }
    else qWarning() << "Vertex Shader source file " << vshader << " not found.";

    // load and compile fragment shader
    QFileInfo fsh(fshader);
    if(fsh.exists())
        {
        FragmentShader = new QGLShader(QGLShader::Fragment);
        if(FragmentShader->compileSourceFile(fshader))
            ShaderProgram->addShader(FragmentShader);
        else qWarning() << "Fragment Shader Error" << FragmentShader->log();
        }
    else qWarning() << "Fragment Shader source file " << fshader << " not found.";

    if(!ShaderProgram->link())
        {
        qWarning() << "Shader Program Linker Error" << ShaderProgram->log();
        }
    else {
        ShaderProgram->bind();
        ShaderProgram->bindAttributeLocation("vertexPosition", 0);
        ShaderProgram->bindAttributeLocation("vertexNormal", 2);
        ShaderProgram->bindAttributeLocation("vertexCoord", 3);
        ShaderProgram->bindAttributeLocation("vertexSecondCoord", 4);
        ShaderProgram->bindAttributeLocation("vertexColor", 5);
    }
}

void shader::bind() {
    ShaderProgram->bind();
}

void shader::release() {
    ShaderProgram->release();
}

GLuint shader::uniformLocation(char *name) {
    return ShaderProgram->uniformLocation(name);
}

GLuint shader::attrLocation(char *name) {
    return ShaderProgram->attributeLocation(name);
}

shader::~shader() {
}

#endif

#ifdef Q_OS_MAC
shader::shader() {
    positionLoc = 0;
    normalLoc = 2;
    coordLoc = 3;
    coordSecondLoc = 4;
    colorLoc = 5;
}

shader::shader(QString vshader, QString fshader) {
    positionLoc = 0;
    normalLoc = 2;
    coordLoc = 3;
    coordSecondLoc = 4;
    colorLoc = 5;
    loadShader(vshader, fshader);
}

void shader::loadShader(QString vshader, QString fshader) {

    // New Stuff
    programObject  = glCreateProgram();
    vertexShader   = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    const char *vertexSource = NoLimitsRenderer::readFile(NoLimitsRenderer::toChar(vshader));
    const char *fragmentSource = NoLimitsRenderer::readFile(NoLimitsRenderer::toChar(fshader));

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Vertex Shader
    qDebug("Compiling shader : %s", NoLimitsRenderer::toChar(vshader));
    glShaderSource(vertexShader, 1, &vertexSource , NULL);
    glCompileShader(vertexShader);

    // Check Vertex Shader
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &InfoLogLength);

    if(InfoLogLength) {
        std::vector<char> VertexShaderErrorMessage(InfoLogLength);
        glGetShaderInfoLog(vertexShader, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        qDebug("%s", &VertexShaderErrorMessage[0]);
    }

    // Compile Fragment Shader
    qDebug("Compiling shader : %s", NoLimitsRenderer::toChar(fshader));
    glShaderSource(fragmentShader, 1, &fragmentSource , NULL);
    glCompileShader(fragmentShader);

    // Check Fragment Shader
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &InfoLogLength);

    if(InfoLogLength) {
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
        glGetShaderInfoLog(fragmentShader, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        qDebug("%s", &FragmentShaderErrorMessage[0]);
    }
    // Link the program
    qDebug("Linking program");
    glAttachShader(programObject, vertexShader);
    glAttachShader(programObject, fragmentShader);

    glBindAttribLocation(programObject, positionLoc, "vertexPosition");
    glBindAttribLocation(programObject, normalLoc, "vertexNormal");
    glBindAttribLocation(programObject, coordLoc, "vertexCoord");
    glBindAttribLocation(programObject, coordSecondLoc, "vertexSecondCoord");
    glBindAttribLocation(programObject, colorLoc, "vertexColor");

    glLinkProgram(programObject);

    // Check the program
    glGetProgramiv(programObject, GL_LINK_STATUS, &Result);
    glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> ProgramErrorMessage( std::max(InfoLogLength, int(1)) );
    glGetProgramInfoLog(programObject, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    qDebug("%s", &ProgramErrorMessage[0]);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void shader::bind() {
    glUseProgram(programObject);
}

void shader::release() {
    glUseProgram(0);
}

GLuint shader::uniformLocation(char *name) {
    if(_unifromBuffer[std::string(name)]) return _unifromBuffer[std::string(name)];
    else return glGetUniformLocation(programObject, name);
}

GLuint shader::attrLocation(char *name) {
    return glGetAttribLocation(programObject, name);
}

shader::~shader() {
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(programObject);
}
#endif
