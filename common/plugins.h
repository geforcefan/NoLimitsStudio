#ifndef PLUGINS_H
#define PLUGINS_H

#include <vector>
#include <map>
#include <sdk/NoLimitsStudio.h>

class plugins : public QObject
{
    Q_OBJECT
public:
    plugins();
    bool loadPlugins();
    void handlePlugin(char* plugin);
    NoLimitsStudio::PluginInterface *getPlugin(char* plugin);
    void reinitPlugin(char* plugin);
    void reinitPlugins();
    void initGL();
    void loadedNewNoLimitsTrack();
    bool eventSend(QObject *obj, QEvent *event);

    std::vector <NoLimitsStudio::PluginInterface*> loadedPlugins;
    std::map <char*, int> Plugins;

public slots:
    void updateRenderer();
};

#endif // PLUGINS_H
