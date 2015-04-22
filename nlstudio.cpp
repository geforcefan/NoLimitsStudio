#include <window/windowmain.h>
#include <common/global.h>
#include <common/plugins.h>

#include <iostream>
#include <maingl/glwidget.h>

#include <QApplication>
#include <QTextStream>

#include <QFont>

void customMessageHandler(QtMsgType type, const char *msg)
{
    QString txt;
    switch (type) {
    case QtDebugMsg:
        txt = QString("Debug: %1\r\n").arg(msg);
        break;

    case QtWarningMsg:
        txt = QString("Warning: %1\r\n").arg(msg);
    break;
    case QtCriticalMsg:
        txt = QString("Critical: %1\r\n").arg(msg);
    break;
    case QtFatalMsg:
        txt = QString("Fatal: %1\r\n").arg(msg);
        abort();
    }

    QFile outFile("debuglog.txt");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << endl;
}

#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //Lets register our custom handler, before we start
    //qInstallMsgHandler(customMessageHandler);

    QGLFormat fmt;
    fmt.setProfile(QGLFormat::CoreProfile);
    fmt.setVersion(3,2);
    fmt.setSampleBuffers(true);
    fmt.setSamples(4);
    QGLFormat::setDefaultFormat(fmt);

    QFont newFont("Sans", 11);
    QApplication::setFont(newFont);
    QApplication::setDesktopSettingsAware(false);

    NoLimitsRenderer::initGlobal();
    NoLimitsRenderer::pluginsLoaded = false;

    NoLimitsRenderer::mainWindow = new windowMain();
    NoLimitsRenderer::mainWindow->showMaximized();

    NoLimitsRenderer::mainWindow->initRenderer();
    //NoLimitsRenderer::Plugins->loadedPlugins[0]->initMode();

    return a.exec();
}
