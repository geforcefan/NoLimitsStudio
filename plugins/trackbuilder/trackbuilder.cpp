#include "trackbuilder.h"

#include <QDockWidget>
#include <QSpinBox>

#include "qWidgets/qfvdcurve.h"
#include "qWidgets/qsectionlist.h"

#include "common/saveloadhandler.h"

#include <maingl/glrenderer.h>
#include "renderer/rendererTrackBuilder.h"

#include "common/eventhandler.h"
#include "common/trackcamera.h"

TrackBuilder *trackBuilder;

RendererThread::RendererThread() { running = false; }
RendererThread::~RendererThread() {}
void RendererThread::run() {
    while(1) {
        if(running) {
            trackBuilder->Mutex->lock();
            trackBuilder->RendererTrackBuilder->update();
            trackBuilder->Mutex->unlock();
            running = false;
        }
        msleep(50);
    }
}


TrackUpdateThread::TrackUpdateThread() { running = false; Callback = NULL; hasCallback = false; hasSection = false; }
TrackUpdateThread::~TrackUpdateThread() {}
void TrackUpdateThread::run() {
    while(1) {
        if(running) {
            trackBuilder->Mutex->lock();

            int secIndex = 0;
            if(hasSection) secIndex = trackBuilder->BuilderTrack->getSectionNumber(sec);
            if(secIndex < 0) secIndex = 0;

            trackBuilder->BuilderTrack->updateTrack(secIndex, 0);
            trackBuilder->Mutex->unlock();

            running = false;

            if(hasCallback) Callback->Execute();
            trackBuilder->trackCamera->moveForwards(false, true);
            trackBuilder->valuesChanged();
        }
        msleep(50);
    }
}

TrackBuilder::TrackBuilder() {
    PluginIdentifier = PLUGINIDENTIFIER;
    PluginName = "Track Builder";
    PluginAuthor = "Søren Have and Ercan Akyürek";
    PluginDescription = "A track builder based on FVD";
    PluginVersion = "1.0.0";
    PluginURL = "http://nolimitstudio.ercanakyuerek.de";

    Mutex = new QMutex();
    BuilderTrack = new track(glm::vec3(0.0f, 5.0f, 0.0f), 0.0f, 1.30f);

    // TrackCamera
    trackCamera = new TrackCamera(NoLimitsRenderer::glrenderer->glt->glw);

    Events = new EventHandler();
    SaveLoadHandler = new saveloadHandler();

    startRenderer = false;
    graphChanges = false;

    // generic settings
    showHeartline = true;
    visualizeForces = false;

    // Build some menu items
    actionShowHeartline = new QAction(tr("&Show Heartline"), this);
    actionShowHeartline->connect(actionShowHeartline, SIGNAL(triggered()), this, SLOT(slotShowHeartline()) );
    actionShowHeartline->setCheckable(true);
    actionShowHeartline->setChecked(showHeartline);

    actionVisualizeForces = new QAction(tr("&Visualize Forces"), this);
    actionVisualizeForces->connect(actionVisualizeForces, SIGNAL(triggered()), this, SLOT(slotVisualizeForces()) );
    actionVisualizeForces->setCheckable(true);
    actionVisualizeForces->setChecked(visualizeForces);

    actionExportOptions = new QAction(tr("&Export Options"), this);
    actionExportOptions->connect(actionExportOptions, SIGNAL(triggered()), this, SLOT(slotExportOptions()) );

    // Renderer Thread
    rendererThread = new RendererThread();
    trackUpdateThread = new TrackUpdateThread();
    trackUpdateThread->start();

    timerAddTrackBuilderRenderer = new QTimer();
    timerAddTrackBuilderRenderer->setInterval(100);
    QObject::connect(timerAddTrackBuilderRenderer, SIGNAL(timeout()), this, SLOT(timerAddTrackBuilderRendererDone()));
    timerAddTrackBuilderRenderer->start();

    tmpStepsize = 0.8f;
}

void TrackBuilder::initPlugin() {
    trackBuilder = this;

    widgetSectionList = new qSectionList(0, this);

    dockSectionList = new QDockWidget(tr("Sections"), mainWindow);
    dockSectionList->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
    dockSectionList->setWidget(widgetSectionList);

    NoLimitsRenderer::currentShader = this->currentShader;
    NoLimitsRenderer::Settings = this->Settings;
    NoLimitsRenderer::Track = (*Track);

    initMode();
}

void TrackBuilder::initGL() {
    RendererTrackBuilder = new rendererTrackBuilder();
    this->glr->addRenderer(RendererTrackBuilder);
    startRenderer = true;
}

void TrackBuilder::timerAddTrackBuilderRendererDone() {
    if(startRenderer) {
        rendererThread->start();
        timerAddTrackBuilderRenderer->stop();
    }
}

void TrackBuilder::slotShowHeartline() {
    showHeartline = showHeartline ? false : true;
}

void TrackBuilder::slotVisualizeForces() {
    visualizeForces = visualizeForces ? false : true;
    rendererThread->running = true;
}

void TrackBuilder::valuesChanged() {
    rendererThread->running = true;
}

void TrackBuilder::updateWholeRenderer() {
    emit updateRenderer();
}

void TrackBuilder::initMode() {
    mainWindow->ui->menuView->addAction(dockSectionList->toggleViewAction());
    mainWindow->ui->menuDisplay->addAction(actionShowHeartline);
    mainWindow->ui->menuDisplay->addAction(actionVisualizeForces);
    mainWindow->ui->menuDisplay->addAction(actionExportOptions);

    mainWindow->addDockWidget(Qt::RightDockWidgetArea, dockSectionList);
    dockSectionList->show();
}

void TrackBuilder::destroyMode() {
    mainWindow->ui->menuView->removeAction(dockSectionList->toggleViewAction());
    mainWindow->ui->menuDisplay->removeAction(actionShowHeartline);
    mainWindow->ui->menuDisplay->removeAction(actionVisualizeForces);
    mainWindow->ui->menuDisplay->removeAction(actionExportOptions);

    mainWindow->removeDockWidget(dockSectionList);
}

void TrackBuilder::initContent() {
}

void TrackBuilder::handleFileSave(pugi::xml_node node) {
    SaveLoadHandler->setNode(node);
    SaveLoadHandler->save();
}

void TrackBuilder::handleFileLoad(pugi::xml_node node) {
    SaveLoadHandler->setNode(node);
    SaveLoadHandler->open();
}

bool TrackBuilder::eventSend(QObject *obj, QEvent *event) {
    Events->receiveEvent(obj, event);
}

void TrackBuilder::slotExportOptions() {

}
