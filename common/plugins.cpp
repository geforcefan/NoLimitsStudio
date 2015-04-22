#include <common/global.h>
#include "plugins.h"

#include <QtCore>
#include <QAction>

#include <window/windowmain.h>
#include "ui_windowmain.h"

#include <maingl/glrenderer.h>
#include <maingl/glthread.h>
#include <maingl/glwidget.h>

#include "plugins/trackbuilder/trackbuilder.h"

plugins::plugins()
{
    loadPlugins();
    NoLimitsRenderer::pluginsLoaded = true;
}

bool plugins::loadPlugins()
{
    /*
    QDir pluginsDir(qApp->applicationDirPath());
#if defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS") {
        pluginsDir.cdUp();
    }
#endif
    pluginsDir.cd("Plugins/");
*/

    //QDir pluginsDir("C:\\Users\\Ercan\\Desktop\\nlstudio\\plugins\\track\\trackbuilder-build-Desktop_Qt_4_8_3-Release\\release\\");
    //QDir pluginsDir("C:\\Users\\Ercan\\Desktop\\nlstudio\\plugins\\track\\trackbuilder-build-Qt_4_8_3-Debug\\debug\\");
    /*QDir pluginsDir("/Users/ercanakyurek/Desktop/nlstudio/plugins/track/build-trackbuilder-Qt_4_8_4_System-Debug/");
    qDebug("Going to look for plugins in: %s", NoLimitsRenderer::toChar(pluginsDir.path()));

    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginLoader.instance();
        this->connect(plugin, SIGNAL(updateRenderer()), this, SLOT(updateRenderer()));
        if (plugin) {
            NoLimitsStudio::PluginInterface *pluginInterface = qobject_cast<NoLimitsStudio::PluginInterface *>(plugin);
            if (pluginInterface) {
                qDebug("Plugin loaded succesfully: %s", NoLimitsRenderer::toChar(pluginsDir.absoluteFilePath(fileName)));

                // Assign global stuff
                qDebug("Pushing back %s with %d", pluginInterface->PluginIdentifier, loadedPlugins.size());
                Plugins[pluginInterface->PluginIdentifier] = loadedPlugins.size();
                loadedPlugins.push_back(pluginInterface);
                this->handlePlugin(pluginInterface->PluginIdentifier);
            }
        }
    }*/

    TrackBuilder *tb = new TrackBuilder();
    this->connect(tb, SIGNAL(updateRenderer()), this, SLOT(updateRenderer()));
    NoLimitsStudio::PluginInterface *pluginInterface = qobject_cast<NoLimitsStudio::PluginInterface *>(tb);
    Plugins[pluginInterface->PluginIdentifier] = loadedPlugins.size();
    loadedPlugins.push_back(pluginInterface);
    this->handlePlugin(pluginInterface->PluginIdentifier);

    return true;
}

NoLimitsStudio::PluginInterface *plugins::getPlugin(char* plugin) {
    std::map<char*,int>::iterator it;

    int pluginIndex = 0;
    for ( it=Plugins.begin() ; it != Plugins.end(); it++ ) {
        if(QString((*it).first) == QString(plugin)) {
            pluginIndex = (*it).second;
        }
    }

    return loadedPlugins[pluginIndex];
}

void plugins::handlePlugin(char* plugin) {
    NoLimitsStudio::PluginInterface *pluginInterface = getPlugin(plugin);
    reinitPlugin(plugin);
    pluginInterface->mainWindow = NoLimitsRenderer::mainWindow;
    pluginInterface->glr = NoLimitsRenderer::glrenderer;
    pluginInterface->currentShader = NoLimitsRenderer::currentShader;
    pluginInterface->Track = &NoLimitsRenderer::Track;
    pluginInterface->initPlugin();
    NoLimitsRenderer::mainWindow->setMode(pluginInterface);
}

void plugins::reinitPlugin(char* plugin) {
    NoLimitsStudio::PluginInterface *pluginInterface = getPlugin(plugin);
    pluginInterface->SaveHandler = NoLimitsRenderer::SaveHandler;
    pluginInterface->Settings = NoLimitsRenderer::Settings;
    pluginInterface->glr = NoLimitsRenderer::glrenderer;
    pluginInterface->currentShader = NoLimitsRenderer::currentShader;
}

void plugins::reinitPlugins() {
    std::map<char*,int>::iterator it;
    for ( it=Plugins.begin() ; it != Plugins.end(); it++ ) {
        reinitPlugin((*it).first);
    }
}

void plugins::initGL() {
    std::map<char*,int>::iterator it;
    for ( it=Plugins.begin() ; it != Plugins.end(); it++ ) {
        NoLimitsStudio::PluginInterface *pluginInterface = getPlugin((*it).first);
        pluginInterface->initGL();
    }
}

bool plugins::eventSend(QObject *obj, QEvent *event) {
    std::map<char*,int>::iterator it;
    for ( it=Plugins.begin() ; it != Plugins.end(); it++ ) {
        NoLimitsStudio::PluginInterface *pluginInterface = getPlugin((*it).first);
        pluginInterface->eventSend(obj, event);
    }
}

void plugins::loadedNewNoLimitsTrack() {
    std::map<char*,int>::iterator it;
    for ( it=Plugins.begin() ; it != Plugins.end(); it++ ) {
        NoLimitsStudio::PluginInterface *pluginInterface = getPlugin((*it).first);
        pluginInterface->loadedNewNoLimitsTrack();
    }
}

/* Slots */
void plugins::updateRenderer() {
    NoLimitsRenderer::glrenderer->update();
}
