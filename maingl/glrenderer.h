#ifndef GLRENDERER_H
#define GLRENDERER_H

#include <common/global.h>
#include <iostream>

#include <QThread>

class GLThread;
class renderer;
class shader;
class lights;
class GLRenderer;

class GLRendererThread : public QThread
{
    Q_OBJECT
public:
    GLRendererThread(QObject *parent, GLRenderer *_glr, int _rendererIndex);
    ~GLRendererThread();
protected:
    void run();
private:
    GLRenderer *glr;
    int rendererIndex;
};

class GLRenderer : public QObject
{
    Q_OBJECT
public:
    GLRenderer(GLThread *_glt);
    void init();
    void draw();
    void update();
    void resize(float _width, float _height);
    void addRenderer(renderer *rnd);

    // Shaders
    shader *MainShader;

    // GL Thread
    GLThread *glt;

    // Subrenderer
    std::vector<renderer*> Renderer;
    int numRenderer;
    bool allowRender;

    // Renderer Thread
    std::vector <GLRendererThread*> glrt;
private:
    void drawObjects();
    void drawBlur();
    void drawShadow();
    void drawScene();

    // Dimensions
    int width, height;
};

#endif // GLRENDERER_H
