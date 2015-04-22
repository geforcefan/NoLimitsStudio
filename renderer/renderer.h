#ifndef RENDERER_H
#define RENDERER_H

#include <common/global.h>

#include <QObject>
#include <QThread>

class renderer
{
public:
    renderer();
    ~renderer();
    virtual void init();
    virtual void render();
    virtual void update();
protected:
    bool allowRender;
    int completeThreads;
    bool changes;
};

#endif // RENDERER_H
