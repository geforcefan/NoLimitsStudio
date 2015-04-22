#include "sectionwidgetstraight.h"
#include "ui_sectionwidgetstraight.h"

#include "../secstraight.h"
#include "../track.h"

#include "trackbuilder.h"

#include <QDebug>

#include <qWidgets/qfvdcurve.h>
#include <qWidgets/qpointplot.h>
#include <qWidgets/qcustomplot.h>

SectionWidgetStraight::SectionWidgetStraight(section *_Section, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SectionWidgetStraight),
    SectionWidget(_Section)
{
    ui->setupUi(this);

    SectionStraight = static_cast<secstraight*>(Section);

    ui->inputStartSpeed->setVisible(false);
    ui->useStartSpeed->setVisible(false);

    // Section Settings
    SectionSettingsDock = new QDockWidget("Straight Section Settings", trackBuilder->mainWindow);
    SectionSettingsDock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
    SectionSettingsDock->setWidget(this);

    CurveRollDock = new QDockWidget(tr("Roll"), trackBuilder->mainWindow);
    CurveRollDock->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
    CurveRollDock->setWidget(SectionStraight->CurveRoll);

    QObject::connect(SectionStraight->CurveRoll, SIGNAL(valuesChanged()), this, SLOT(valuesChanged()));

    if(SectionStraight->bSpeed == 0) ui->useFixedSpeed->setChecked(SectionStraight->bSpeed == 0?true:false);
    ui->inputSpeed->setValue(SectionStraight->fVel);

    // Timer for reploting graphes
    GraphReplotTimer = new QTimer();
    GraphReplotTimer->setInterval(30);
    QObject::connect(GraphReplotTimer, SIGNAL(timeout()), this, SLOT(GraphReplotTimerDone()));
}

void SectionWidgetStraight::valuesChanged() {
    Update();
}

void SectionWidgetStraight::initInterface() {
    GraphReplotTimer->start();

    trackBuilder->mainWindow->addDockWidget(Qt::LeftDockWidgetArea, SectionSettingsDock);
    trackBuilder->mainWindow->addDockWidget(Qt::BottomDockWidgetArea, CurveRollDock);
    trackBuilder->mainWindow->tabifyDockWidget(SectionSettingsDock, SectionInfoDock);

    SectionSettingsDock->show();
    SectionInfoDock->show();
    CurveRollDock->show();
}

void SectionWidgetStraight::clearInterface() {
    GraphReplotTimer->stop();

    SectionSettingsDock->hide();
    CurveRollDock->hide();
    SectionInfoDock->hide();
}

void SectionWidgetStraight::Update() {
    SectionStraight->CurveRoll->replot();

    SectionWidget::Update();
}

void SectionWidgetStraight::GraphReplotTimerDone() {
    if(!trackBuilder->graphChanges) return;
    SectionStraight->CurveRoll->replot();
    trackBuilder->graphChanges = false;
}

void SectionWidgetStraight::on_useFixedSpeed_clicked(bool checked)
{
    SectionStraight->bSpeed = checked?0:1;
    ui->inputSpeed->setEnabled(checked);

    Update();
}

void SectionWidgetStraight::on_inputSpeed_valueChanged(double arg1)
{
    SectionStraight->fVel = arg1;
    Update();
}

SectionWidgetStraight::~SectionWidgetStraight()
{
    delete ui;
}

void SectionWidgetStraight::on_useStartSpeed_clicked(bool checked)
{
    SectionStraight->useStartSpeed = checked?true:false;
    ui->inputStartSpeed->setEnabled(checked);
    Update();
}

void SectionWidgetStraight::on_inputStartSpeed_valueChanged(double arg1)
{
    SectionStraight->fVel = arg1;
    Update();
}

void SectionWidgetStraight::on_showRoll_clicked(bool checked)
{
    SectionStraight->showRoll = checked;
    SectionStraight->updateGraphStates();
    SectionStraight->CurveRoll->pointPlot->plotter->replot();
}

void SectionWidgetStraight::on_showShowRollDerivative_clicked(bool checked)
{
    SectionStraight->showShowRollDerivative = checked;
    SectionStraight->updateGraphStates();
    SectionStraight->CurveRoll->pointPlot->plotter->replot();
}

void SectionWidgetStraight::on_showVerticalForce_clicked(bool checked)
{

    SectionStraight->showVerticalForce = checked;
    SectionStraight->updateGraphStates();
    SectionStraight->CurveRoll->pointPlot->plotter->replot();
}

void SectionWidgetStraight::on_showLateralForce_clicked(bool checked)
{
    SectionStraight->showLateralForce = checked;
    SectionStraight->updateGraphStates();
    SectionStraight->CurveRoll->pointPlot->plotter->replot();
}
