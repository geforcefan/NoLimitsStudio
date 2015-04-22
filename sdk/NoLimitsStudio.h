#ifndef NOLIMITSSTUDIO_H
#define NOLIMITSSTUDIO_H

#include <common/global.h>
#include <QSettings>

#include <pugixml/pugixml.hpp>

class windowMain;
class saveHandler;
class GLRenderer;
class shader;
class settings;
class Banking;

namespace NoLimitsStudio {
    class PluginInterface {
    public:
        virtual ~PluginInterface() {}
        virtual void initPlugin() = 0;

        // Handle mode switch
        virtual void initMode() = 0;
        virtual void destroyMode() = 0;

        // Handle file saving and loading
        virtual void handleFileSave(pugi::xml_node node) = 0;
        virtual void handleFileLoad(pugi::xml_node node) = 0;

        // Reinit
        virtual void initContent() = 0;
        virtual void initGL() = 0;

        // Init New NoLimits File
        virtual void loadedNewNoLimitsTrack() = 0;

        // Events
        virtual bool eventSend(QObject *obj, QEvent *event) = 0;

        // Plugin Information
        char *PluginIdentifier;
        char *PluginName;
        char *PluginAuthor;
        char *PluginDescription;
        char *PluginVersion;
        char *PluginURL;

        // Global stuff
        windowMain *mainWindow;
        GLRenderer *glr;
        saveHandler *SaveHandler;
        shader *currentShader;
        NlTrack *Track;
        settings *Settings;

        Banking *TrackBanking;
    };
}
Q_DECLARE_INTERFACE(NoLimitsStudio::PluginInterface, "com.NoLimitsStudio.PluginInterface/1.0")

#endif // NOLIMITSSTUDIO_H
