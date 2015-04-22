#include "sectionwidgetcurved.h"
#include "ui_sectionwidgetcurved.h"

#include "../seccurved.h"
#include "../track.h"

#include "trackbuilder.h"

#include <qWidgets/qfvdcurve.h>
#include <qWidgets/qpointplot.h>
#include <qWidgets/qcustomplot.h>

SectionWidgetCurved::SectionWidgetCurved(section *_Section, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SectionWidgetCurved),
    SectionWidget(_Section)
{
    ui->setupUi(this);

    SectionCurved = static_cast<seccurved*>(Section);

    // Section Settings
    SectionSettingsDock = new QDockWidget("Curved Section Settings", trackBuilder->mainWindow);
    SectionSettingsDock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
    SectionSettingsDock->setWidget(this);

    CurveRollDock = new QDockWidget(tr("Roll"), trackBuilder->mainWindow);
    CurveRollDock->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
    CurveRollDock->setWidget(SectionCurved->CurveRoll);

    QObject::connect(SectionCurved->CurveRoll, SIGNAL(valuesChanged()), this, SLOT(valuesChanged()));

    if(SectionCurved->bSpeed == 0) ui->useFixedSpeed->setChecked(SectionCurved->bSpeed == 0?true:false);
    ui->inputSpeed->setValue(SectionCurved->fVel);

    ui->inputHeartline->setValue(SectionCurved->fRadius);
    ui->inputDirection->setValue(SectionCurved->fDirection);
    ui->inputLeadIn->setValue(SectionCurved->fLeadIn);
    ui->inputLeadOut->setValue(SectionCurved->fLeadOut);

    // Timer for reploting graphes
    GraphReplotTimer = new QTimer();
    GraphReplotTimer->setInterval(30);
    QObject::connect(GraphReplotTimer, SIGNAL(timeout()), this, SLOT(GraphReplotTimerDone()));
}

void SectionWidgetCurved::valuesChanged() {
    Update();
}

void SectionWidgetCurved::initInterface() {
    GraphReplotTimer->start();

    trackBuilder->mainWindow->addDockWidget(Qt::LeftDockWidgetArea, SectionSettingsDock);
    trackBuilder->mainWindow->addDockWidget(Qt::BottomDockWidgetArea, CurveRollDock);
    trackBuilder->mainWindow->tabifyDockWidget(SectionSettingsDock, SectionInfoDock);

    SectionSettingsDock->show();
    SectionInfoDock->show();
    CurveRollDock->show();
}

void SectionWidgetCurved::clearInterface() {
    GraphReplotTimer->stop();

    SectionSettingsDock->hide();
    CurveRollDock->hide();
    SectionInfoDock->hide();
}

void SectionWidgetCurved::Update() {
    SectionCurved->CurveRoll->replot();

    SectionWidget::Update();
}

void SectionWidgetCurved::GraphReplotTimerDone() {
    if(!trackBuilder->graphChanges) return;
    SectionCurved->CurveRoll->replot();
    trackBuilder->graphChanges = false;
}

SectionWidgetCurved::~SectionWidgetCurved()
{
    delete ui;
}


void SectionWidgetCurved::on_useFixedSpeed_clicked(bool checked)
{
    SectionCurved->bSpeed = checked?0:1;
    ui->inputSpeed->setEnabled(checked);

    Update();
}

void SectionWidgetCurved::on_inputSpeed_valueChanged(double arg1)
{
    SectionCurved->fVel = arg1;
    Update();
}

void SectionWidgetCurved::on_inputHeartline_valueChanged(double arg1)
{
    SectionCurved->fRadius = arg1;
    Update();
}

void SectionWidgetCurved::on_inputDirection_valueChanged(double arg1)
{
    SectionCurved->fDirection = arg1;
    Update();
}

void SectionWidgetCurved::on_inputLeadIn_valueChanged(double arg1)
{
    SectionCurved->fLeadIn = arg1;
    Update();
}

void SectionWidgetCurved::on_inputLeadOut_valueChanged(double arg1)
{
    SectionCurved->fLeadOut = arg1;
    Update();
}

void SectionWidgetCurved::on_showRoll_clicked(bool checked)
{
    SectionCurved->showRoll = checked;
    SectionCurved->updateGraphStates();
    SectionCurved->CurveRoll->pointPlot->plotter->replot();
}

void SectionWidgetCurved::on_showShowRollDerivative_clicked(bool checked)
{
    SectionCurved->showShowRollDerivative = checked;
    SectionCurved->updateGraphStates();
    SectionCurved->CurveRoll->pointPlot->plotter->replot();
}

void SectionWidgetCurved::on_showVerticalForce_clicked(bool checked)
{

    SectionCurved->showVerticalForce = checked;
    SectionCurved->updateGraphStates();
    SectionCurved->CurveRoll->pointPlot->plotter->replot();
}

void SectionWidgetCurved::on_showLateralForce_clicked(bool checked)
{
    SectionCurved->showLateralForce = checked;
    SectionCurved->updateGraphStates();
    SectionCurved->CurveRoll->pointPlot->plotter->replot();
}
