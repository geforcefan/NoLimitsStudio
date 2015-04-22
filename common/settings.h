#ifndef SETTINGS_H
#define SETTINGS_H

class QSettings;

#include <QString>

class settings
{
public:
    settings();
    void saveSettings();

    bool rendererCadMode;
    bool rendererSky;
    bool rendererTerrain;
    bool rendererTrack;
    bool rendererGround;
    bool rendererSupports;

    QString NoLimitsDir;
private:
    QSettings *handler;
};

#endif // SETTINGS_H
