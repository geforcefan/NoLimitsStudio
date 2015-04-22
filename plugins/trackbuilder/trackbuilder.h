#ifndef TRACKBUILDER_H
#define TRACKBUILDER_H

#include "trackbuilder_global.h"

#include <qplugin.h>
#include <sdk/NoLimitsStudio.h>

#include <window/windowmain.h>
#include "ui_windowmain.h"

#include <maingl/glrenderer.h>
#include <maingl/glthread.h>

#include "track/track.h"
#include <QMutex>

#include <iostream>
#include <ostream>
#include <functional>

#define PLUGINIDENTIFIER "TrackBuilder"

class QDockWidget;
class qSectionList;
class rendererTrackBuilder;
class rendererTrack;
class EventHandler;
class saveloadHandler;
class TrackCamera;

class RendererThread : public QThread
{
    Q_OBJECT
public:
    RendererThread();
    ~RendererThread();
    bool running;

protected:
    void run();
};

class TrackUpdateThreadCallback {
public:
    TrackUpdateThreadCallback() {}
    virtual void Execute() = 0;
};

class TrackUpdateThread : public QThread
{
    Q_OBJECT
public:
    TrackUpdateThread();
    ~TrackUpdateThread();
    bool running;

    TrackUpdateThreadCallback *Callback;
    section *sec;

    bool hasCallback;
    bool hasSection;

    void setCallback(TrackUpdateThreadCallback *cb) {
        Callback = cb;
        hasCallback = true;
    }

    void setSection(section *_sec) {
        sec = _sec;
        hasSection = true;
    }

protected:
    void run();
};

class TRACKBUILDER_EXPORT TrackBuilder : public QObject, public NoLimitsStudio::PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(NoLimitsStudio::PluginInterface)

public:
    TrackBuilder();
    void initPlugin();

    void initMode();
    void initGL();
    void destroyMode();

    void handleFileSave(pugi::xml_node node);
    void handleFileLoad(pugi::xml_node node);

    void initContent();

    void loadedNewNoLimitsTrack() {}

    // Events
    bool eventSend(QObject *obj, QEvent *event);

    void updateWholeRenderer();

    qSectionList *widgetSectionList;
    rendererTrackBuilder *RendererTrackBuilder;

    RendererThread *rendererThread;
    TrackUpdateThread *trackUpdateThread;

    saveloadHandler *SaveLoadHandler;

    bool showHeartline;
    bool visualizeForces;

    bool startRenderer;
    bool graphChanges;

    QTimer *timerAddTrackBuilderRenderer;

    EventHandler *Events;

    QDockWidget *dockSectionList;

    float tmpStepsize;

    track *BuilderTrack;

    QMutex *Mutex;

    TrackCamera *trackCamera;
private:

    // menu actions
    QAction *actionShowHeartline;
    QAction *actionVisualizeForces;
    QAction *actionExportOptions;

public slots:
    void valuesChanged();
    void slotShowHeartline();
    void slotVisualizeForces();
    void timerAddTrackBuilderRendererDone();
    void slotExportOptions();

signals:
    void updateRenderer();
};

extern TrackBuilder *trackBuilder;

#endif // TRACKBUILDER_H
