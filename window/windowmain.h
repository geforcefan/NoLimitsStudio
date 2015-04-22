#ifndef WINDOWMAIN_H
#define WINDOWMAIN_H

#include <common/global.h>

#include <QMainWindow>
#include <QWidget>

#include <maingl/glwidget.h>
#include <maingl/glthread.h>

namespace Ui {
    class windowMain;
}

namespace NoLimitsStudio {
    class PluginInterface;
}

class sideWidget;
class QCloseEvent;
class QSignalMapper;

class windowMain : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit windowMain(QWidget *parent = 0);
    ~windowMain();
    bool eventFilter(QObject *obj, QEvent *event);

    void initRenderer();
    void setMode(NoLimitsStudio::PluginInterface *pluginInterface);
    void initContent();

    bool saveDialog();
    void saveFile();

    void initWindowTitle();
    void setChanges(bool state);
    bool isNlTrackEmpty();
    void openNLTrack(char *filename);

    void checkStartSimulator();
    QString Nolimitize();

    Ui::windowMain *ui;

    char *currentFile;
    char *currentTitle;
    bool changes;
    bool importedNLTrack;

    GLWidget* glw;

private slots:
    void on_actionOpen_triggered();
    void on_actionNew_triggered();
    void on_actionImportNL_triggered();
    void on_actionSave_triggered();
    void on_actionSave_As_triggered();
    void on_actionCAD_Mode_triggered(bool s);
    void on_actionShow_Sky_triggered(bool s);
    void on_actionShow_Terrain_triggered(bool s);
    void on_actionShow_Track_triggered(bool s);
    void on_actionShow_Supports_triggered(bool s);
    void on_actionShow_Ground_triggered(bool s);
    void on_actionExport_NoLimits_Track_triggered();
    void on_actionStart_Simulator_triggered();
    void on_actionOptions_triggered();
    void on_actionBake_for_NoLimits_triggered();

    void changeMode(QObject *modeObj);

private:
    QWidget *glWidget;

    sideWidget *sidewidget;

    virtual void closeEvent ( QCloseEvent * event );

    QSignalMapper *modeSignalMapper;
    std::vector<QAction*> modeActions;
};

#endif // WINDOWMAIN_H
