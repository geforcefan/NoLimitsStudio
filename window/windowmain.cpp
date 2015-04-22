#include <common/global.h>

#include <QFile>

#include <common/plugins.h>
#include <common/savehandler.h>
#include <common/settings.h>

#include <maingl/glrenderer.h>
#include <sdk/NoLimitsStudio.h>

#include "windowmain.h"
#include "ui_windowmain.h"

#include <QDockWidget>
#include <QCloseEvent>
#include <QSignalMapper>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>

#include <window/modeobject.h>

#include <QProcess>

#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_7
void* setFullScreen(QMainWindow *mainWindow); // Ab OSX 10.7 gibt es eine tolle fullscreen mode :)
#endif

windowMain::windowMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::windowMain) {
    ui->setupUi(this);

    QHBoxLayout *glgrid = this->findChild<QHBoxLayout*>("GLGrid");

    QGLFormat glFormat = QGLFormat::defaultFormat();
#ifdef Q_OS_WIN
    glFormat.setVersion(3, 1);
    glFormat.setProfile(QGLFormat::CoreProfile);
#endif

    // TODO: get max samples and set if there is samples available
    glFormat.setSampleBuffers( true );
    glFormat.setSamples(4);
    glFormat.setDoubleBuffer(true);
    glFormat.setDirectRendering(true);

    glw = new GLWidget(glFormat, this);
    glgrid->addWidget(glw, 1);
    glw->show();

    installEventFilter(this);

    modeSignalMapper = new QSignalMapper(this);
    connect(modeSignalMapper, SIGNAL(mapped(QObject *)),
            this, SLOT(changeMode(QObject *)));

    // current File
    currentFile = "";
    currentTitle = "NoLimitsStudio";
    changes = false;
    importedNLTrack = false;

    // Init some stuff from settings
    this->ui->actionCAD_Mode->setChecked(NoLimitsRenderer::Settings->rendererCadMode);
    this->ui->actionShow_Track->setChecked(NoLimitsRenderer::Settings->rendererTrack);
    this->ui->actionShow_Terrain->setChecked(NoLimitsRenderer::Settings->rendererTerrain);
    this->ui->actionShow_Ground->setChecked(NoLimitsRenderer::Settings->rendererGround);
    this->ui->actionShow_Sky->setChecked(NoLimitsRenderer::Settings->rendererSky);
    this->ui->actionShow_Supports->setChecked(NoLimitsRenderer::Settings->rendererSupports);

    checkStartSimulator();

    // Mac fullscreen
    #if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_7
    setFullScreen(this);
    #endif
}

QString windowMain::Nolimitize() {
    QDir *qDirOperations = new QDir();

    // Create some vars for dir
    QFileInfo currentFileInfo = QFileInfo(QString(currentFile));
    QString filePath = QString(NoLimitsRenderer::Settings->NoLimitsDir).append("/");

    // NoLimits Directories
    QString pathObject = QString(filePath).append("objects/");
    QString pathTrack = QString(filePath).append("tracks/");

    // Project Directory
    QString pathProject = QString(pathTrack).append( currentFileInfo.fileName().split(".")[0] ).append("/");

    // check some directories
    if(!QDir(pathProject).exists()) {
        qDebug() << "not exist" << pathProject;
        qDirOperations->mkdir(pathProject.replace("/", "\\"));
    }

    QString fileTrack = QString(pathProject).append(currentFileInfo.fileName().split(".")[0]).append(".nltrack");

    NlSave(NoLimitsRenderer::toChar(fileTrack), &NoLimitsRenderer::Track);

    return fileTrack;
}

void windowMain::checkStartSimulator() {
    // Check for NoLimits Root Dir
    if(!NoLimitsRenderer::Settings->NoLimitsDir.length()) {
        on_actionOptions_triggered();
        ui->actionStart_Simulator->setEnabled(false);
        ui->actionBake_for_NoLimits->setEnabled(false);
    } else {
        if(!QString(currentFile).length()) {
            ui->actionStart_Simulator->setEnabled(false);
            ui->actionBake_for_NoLimits->setEnabled(false);
        } else {
            ui->actionStart_Simulator->setEnabled(true);
            ui->actionBake_for_NoLimits->setEnabled(true);
        }
    }
}

void windowMain::initRenderer() {
    //return;
    glw->initRendering();

    NoLimitsRenderer::Plugins = new plugins();
    NoLimitsRenderer::Plugins->reinitPlugins();
}

void windowMain::setMode(NoLimitsStudio::PluginInterface *pluginInterface) {
    QAction* modeAction = new QAction(pluginInterface->PluginName, NoLimitsRenderer::mainWindow);
    modeAction->setCheckable(true);

    modeObject *mObject = new modeObject();
    mObject->action = modeAction;
    mObject->pluginIdentiefier = pluginInterface->PluginIdentifier;

    connect(modeAction, SIGNAL(triggered()), modeSignalMapper, SLOT(map()));
    modeSignalMapper->setMapping(modeAction, mObject);

    NoLimitsRenderer::mainWindow->ui->menuMode->addAction(modeAction);
    modeActions.push_back(modeAction);
}

void windowMain::changeMode(QObject *modeObj) {
    modeObject *mode = qobject_cast<modeObject*>(modeObj);

    for(int i = 0; i < modeActions.size(); i++) modeActions[i]->setChecked(false);
    for(int i = 0; i < NoLimitsRenderer::Plugins->loadedPlugins.size() ; i++) NoLimitsRenderer::Plugins->loadedPlugins[i]->destroyMode();

    NoLimitsRenderer::Plugins->getPlugin(NoLimitsRenderer::toChar(mode->pluginIdentiefier))->initMode();
    mode->action->setChecked(true);
}

bool windowMain::eventFilter(QObject *obj, QEvent *event) {
  //if (event->type() != QEvent::MouseMove) glw->eventFilter(obj, event);

  if(event->type() != 129) {
      glw->eventFilterFake(obj, event);
      if(NoLimitsRenderer::pluginsLoaded) NoLimitsRenderer::Plugins->eventSend(obj, event);
  }

  emit focusWidget();
  emit focusProxy();
  emit focusPolicy();

  return false;
}

windowMain::~windowMain() {
    delete ui;
}

void windowMain::setChanges(bool state) {
    if(state) {
        changes = state;
        setWindowTitle(QString(currentTitle).append("*"));
    } else {
        changes = state;
        setWindowTitle(QString(currentTitle));
    }
}

void windowMain::closeEvent ( QCloseEvent * event ) {
    NoLimitsRenderer::Settings->saveSettings();
    this->glw->getGLThread().stop();
}

void windowMain::initContent() {
    importedNLTrack = false;
    currentFile = "";
    setChanges(false);

    memset(&NoLimitsRenderer::Track, 0, sizeof(NlTrack));
    initNewNlTrack(&NoLimitsRenderer::Track);

    for(int i = 0; i < NoLimitsRenderer::Plugins->loadedPlugins.size(); i++)
        NoLimitsRenderer::Plugins->loadedPlugins[i]->initContent();

    NoLimitsRenderer::initGlobal();
    NoLimitsRenderer::Plugins->reinitPlugins();
}

void windowMain::initWindowTitle() {
    QFileInfo *file = new QFileInfo(currentFile);
    if(strlen(currentFile)) setWindowTitle(QString("NoLimitsStudio - ").append(file->baseName()).append(".").append(file->suffix()));
    else setWindowTitle("NoLimitsStudio");

    currentTitle = NoLimitsRenderer::toChar(this->windowTitle());
}

bool windowMain::saveDialog() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                     "",
                                                     tr("NoLimits Studio Files (*.nlstudio)"));
    if(fileName.length()) {
        currentFile = NoLimitsRenderer::toChar(fileName);
        saveFile();
        return true;
    } else return false;
}

void windowMain::saveFile() {
    setChanges(false);
    NoLimitsRenderer::SaveHandler->performSave(this->currentFile);
    initWindowTitle();

    checkStartSimulator();
}

void windowMain::on_actionOpen_triggered() {
    if(changes) switch( QMessageBox::question( this, "Save changes?",
                                      "The document has been changed since "
                                      "the last save.",
                                      "Cancel", "Leave Anyway", "Save Now",
                                      0, 0 ) ) {
    case 0:
        return;
        break;
    case 2:
        if(!saveDialog()) return;
        break;
    }
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open NoLimitsStudio File"),
                                                     "",
                                                     tr("NoLimitsStudio Files (*.nlstudio)"));
    if(fileName.length()) {
        initContent();
        setChanges(false);
        currentFile = NoLimitsRenderer::toChar(fileName);
        NoLimitsRenderer::SaveHandler->performOpen(this->currentFile);
    }

    initWindowTitle();
    checkStartSimulator();
}

bool windowMain::isNlTrackEmpty() {
    if(
            NoLimitsRenderer::Track.numBeziers > 1 ||
            NoLimitsRenderer::Track.totalSegments ||
            NoLimitsRenderer::Track.sizeX ||
            NoLimitsRenderer::Track.totalFreeNodes ||
            NoLimitsRenderer::Track.totalObjects ||
            NoLimitsRenderer::Track.totalTrees ||
            NoLimitsRenderer::Track.totalTubes ||
            NoLimitsRenderer::Track.totalFundNodes
            ) return false;
    else return true;
}

#include <common/nlsegment.h>

void windowMain::openNLTrack(char *filename) {
    memset(&NoLimitsRenderer::Track, 0, sizeof(NlTrack));
    NlOpen(NoLimitsRenderer::toChar(filename), &NoLimitsRenderer::Track);

    importedNLTrack = true;

/*
    if(NoLimitsRenderer::Track.segs) for(int i=0; i < 3; i++) {
        NLSegment *n = new NLSegment(i);
        n->getOffsetPosition(8.0);
        //qDebug() << "i" << i << n->getLength() << n->lastOffset;
    }

    exit(0);
*/

    this->glw->doneCurrent();
    this->glw->getGLThread().init();
    NoLimitsRenderer::Plugins->loadedNewNoLimitsTrack();
}

void windowMain::on_actionStart_Simulator_triggered() {
    QString fileTrack = Nolimitize();

#ifdef Q_OS_WIN
    QString SystemCall;
    SystemCall = QString("cd %c").append(NoLimitsRenderer::Settings->NoLimitsDir).append("%c && NoLimitsSimulator.exe -s  %c").replace("/", "\\");

    // TODO: System run should be runnable under mac
    QString systemRun = QString("").sprintf(NoLimitsRenderer::toChar(SystemCall), 0x22, 0x22, 0x22);
    systemRun.append(fileTrack.replace("/", "\\"));
    systemRun.append(QString("").sprintf("%c", 0x22));

    system(NoLimitsRenderer::toChar(systemRun));
#endif
#ifdef Q_OS_MAC
    QProcess *Process = new QProcess();
    QString TrackFilePath = fileTrack;
    Process->start(QString(NoLimitsRenderer::Settings->NoLimitsDir).append("/NoLimitsSimulator.app"), (QStringList() << "-s" << TrackFilePath) );
    qDebug() << Process->errorString();
#endif
}

void windowMain::on_actionBake_for_NoLimits_triggered() {
    Nolimitize();
}

void windowMain::on_actionImportNL_triggered()
{
    if(!isNlTrackEmpty()) switch( QMessageBox::warning( this, "Replace NoLimits Track?",
                                              "The document contains already "
                                              "a NoLimits Track.",
                                                      "Cancel", "Replace",
                                              0, 0 ) ) {
            case 0:
                return;
                break;
    }

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open NoLimits Track File"),
                                                     "",
                                                     tr("NoLimits Track Files (*.nltrack)"));
    if(fileName.length()) {
        setChanges(true);
        openNLTrack(NoLimitsRenderer::toChar(fileName));
    }
}

void windowMain::on_actionNew_triggered()
{
    if(changes) switch( QMessageBox::question( this, "Save changes?",
                                      "The document has been changed since "
                                      "the last save.",
                                      "Cancel", "Leave Anyway", "Save Now",
                                      0, 0 ) ) {
    case 0:
        return;
        break;
    case 2:
        saveDialog();
        break;
    }

    initContent();
    glw->getGLThread().init();

    initWindowTitle();
    checkStartSimulator();
}


#include <window/windowsettings.h>

void windowMain::on_actionOptions_triggered() {
    WindowSettings *ws = new WindowSettings();

    ws->show();
}

void windowMain::on_actionExport_NoLimits_Track_triggered() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save NoLimits Track"),
                                                     "",
                                                     tr("NoLimits Track File (*.nltrack)"));
    if(fileName.length()) {
        NlSave(NoLimitsRenderer::toChar(fileName), &NoLimitsRenderer::Track);
        return;
    } else return;
}

void windowMain::on_actionSave_triggered() {
    if(strlen(currentFile)) saveFile();
    else saveDialog();
}

void windowMain::on_actionSave_As_triggered() {
    saveDialog();
}

void windowMain::on_actionCAD_Mode_triggered(bool s) {
    NoLimitsRenderer::Settings->rendererCadMode = s;
}

void windowMain::on_actionShow_Sky_triggered(bool s) {
    NoLimitsRenderer::Settings->rendererSky = s;
}

void windowMain::on_actionShow_Terrain_triggered(bool s) {
    NoLimitsRenderer::Settings->rendererTerrain = s;
}

void windowMain::on_actionShow_Track_triggered(bool s) {
    NoLimitsRenderer::Settings->rendererTrack = s;
}

void windowMain::on_actionShow_Supports_triggered(bool s) {
    NoLimitsRenderer::Settings->rendererSupports = s;
}

void windowMain::on_actionShow_Ground_triggered(bool s) {
    NoLimitsRenderer::Settings->rendererGround = s;
}
