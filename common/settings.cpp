#include "settings.h"

#include <QSettings>
#include <QDebug>

#include <QDesktopServices>

settings::settings()
{
    handler = new QSettings("geforcefan", "NoLimitsStudio");

    handler->beginGroup("NoLimitsStudio");
    NoLimitsDir = handler->value("NoLimitsDir", "").toString();
    handler->endGroup();

    // Init renderer settings
    handler->beginGroup("Display");
    rendererCadMode = handler->value("rendererCadMode", false).toBool();
    rendererSky = handler->value("rendererSky", true).toBool();
    rendererTerrain = handler->value("rendererTerrain", true).toBool();
    rendererTrack = handler->value("rendererTrack", true).toBool();
    rendererSupports = handler->value("rendererSupports", true).toBool();
    rendererGround = handler->value("rendererGround", true).toBool();
    handler->endGroup();
}

void settings::saveSettings() {
    handler = new QSettings("geforcefan", "NoLimitsStudio");

    handler->beginGroup("NoLimitsStudio");
    handler->setValue("NoLimitsDir", NoLimitsDir);
    handler->endGroup();

    handler->beginGroup("Display");
    handler->setValue("rendererCadMode", rendererCadMode);
    handler->setValue("rendererSky", rendererSky);
    handler->setValue("rendererTerrain", rendererTerrain);
    handler->setValue("rendererTrack", rendererTrack);
    handler->setValue("rendererSupports", rendererSupports);
    handler->setValue("rendererGround", rendererGround);
    handler->endGroup();
}
