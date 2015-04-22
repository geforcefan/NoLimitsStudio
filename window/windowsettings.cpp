#include <QLayout>

#include "windowsettings.h"
#include "ui_windowsettings.h"

#include <QMessageBox>
#include <QCloseEvent>
#include <QDir>
#include <QDebug>
#include <QFileDialog>
#include <common/global.h>
#include <common/settings.h>

WindowSettings::WindowSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WindowSettings)
{
    Errors = false;

    ui->setupUi(this);
    ui->NoLimitsRootDir->setText(NoLimitsRenderer::Settings->NoLimitsDir);
    on_NoLimitsRootDir_textEdited(NoLimitsRenderer::Settings->NoLimitsDir);
}

void WindowSettings::closeEvent ( QCloseEvent * event ) {
    if(Errors) {
        accept();
        event->ignore();
    }
}

void WindowSettings::accept() {
    Errors = false;
    if(Errors) {
        QMessageBox::critical(this, "Faulty input", "Please check your options. Some do not seem to be entirely correct");
    } else {
        NoLimitsRenderer::Settings->NoLimitsDir = ui->NoLimitsRootDir->text();
        NoLimitsRenderer::Settings->saveSettings();

        QString NoLimitsStudioDir = NoLimitsRenderer::Settings->NoLimitsDir.append("/NoLimitsStudio/");
        //QDir::mkdir(NoLimitsStudioDir);

        this->close();
        this->destroy();
    }
}

void WindowSettings::on_NoLimitsRootDir_textEdited(const QString &arg1)
{
    bool simExist = false;

    // TODO: Mac have other filenames
#ifdef Q_OS_MAC
    simExist = QDir(QString(arg1).append("/").append("NoLimitsSimulator.app")).exists();
#endif
#ifdef Q_OS_WIN
    simExist = QFile(QString(arg1).append("\\").append("NoLimitsSimulator.exe")).exists();
#endif

    if(arg1.length() && simExist) {
        Errors = false;
        ui->NoLimitsRootDir->setStyleSheet("color: green;");
    } else {
        Errors = true;
        ui->NoLimitsRootDir->setStyleSheet("color: red;");
    }
}

WindowSettings::~WindowSettings()
{
    delete ui;
}

void WindowSettings::on_pushButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select NoLimits Directory"),
                                                ui->NoLimitsRootDir->text(),
                                                QFileDialog::ShowDirsOnly
                                                | QFileDialog::DontResolveSymlinks);
    ui->NoLimitsRootDir->setText(dir);
    on_NoLimitsRootDir_textEdited(dir);
}
