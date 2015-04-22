#include "sectionwidgetforced.h"
#include "ui_sectionwidgetforced.h"

#include "../secforced.h"
#include "../track.h"

#include "trackbuilder.h"

#include <qWidgets/qfvdcurve.h>
#include <qWidgets/qpointplot.h>
#include <qWidgets/qcustomplot.h>

SectionWidgetForced::SectionWidgetForced(section *_Section, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SectionWidgetForced),
    SectionWidget(_Section)
{
    ui->setupUi(this);

    SectionForced = static_cast<secforced*>(Section);

    // Section Settings
    SectionSettingsDock = new QDockWidget("Curved Section Settings", trackBuilder->mainWindow);
    SectionSettingsDock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
    SectionSettingsDock->setWidget(this);

    CurveRollDock = new QDockWidget(tr("Roll"), trackBuilder->mainWindow);
    CurveRollDock->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
    CurveRollDock->setWidget(SectionForced->CurveRoll);

    CurveVertDock = new QDockWidget(tr("Vertical Force"), trackBuilder->mainWindow);
    CurveVertDock->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
    CurveVertDock->setWidget(SectionForced->CurveVert);

    CurveLatDock = new QDockWidget(tr("Lateral Force"), trackBuilder->mainWindow);
    CurveLatDock->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
    CurveLatDock->setWidget(SectionForced->CurveLat);


    QObject::connect(SectionForced->CurveRoll, SIGNAL(valuesChanged()), this, SLOT(valuesChanged()));
    QObject::connect(SectionForced->CurveVert, SIGNAL(valuesChanged()), this, SLOT(valuesChanged()));
    QObject::connect(SectionForced->CurveLat, SIGNAL(valuesChanged()), this, SLOT(valuesChanged()));

    if(SectionForced->bSpeed == 0) ui->useFixedSpeed->setChecked(SectionForced->bSpeed == 0?true:false);
    ui->inputSpeed->setValue(SectionForced->fVel);

    // Timer for reploting graphes
    GraphReplotTimer = new QTimer();
    GraphReplotTimer->setInterval(30);
    QObject::connect(GraphReplotTimer, SIGNAL(timeout()), this, SLOT(GraphReplotTimerDone()));
}

void SectionWidgetForced::valuesChanged() {
    Update();
}

void SectionWidgetForced::initInterface() {
    GraphReplotTimer->start();

    trackBuilder->mainWindow->addDockWidget(Qt::LeftDockWidgetArea, SectionSettingsDock);
    trackBuilder->mainWindow->tabifyDockWidget(SectionSettingsDock, SectionInfoDock);

    trackBuilder->mainWindow->addDockWidget(Qt::BottomDockWidgetArea, CurveVertDock);
    trackBuilder->mainWindow->addDockWidget(Qt::BottomDockWidgetArea, CurveRollDock);
    trackBuilder->mainWindow->addDockWidget(Qt::BottomDockWidgetArea, CurveLatDock);
    trackBuilder->mainWindow->tabifyDockWidget(CurveRollDock, CurveLatDock);

    SectionSettingsDock->show();
    SectionInfoDock->show();
    CurveVertDock->show();
    CurveRollDock->show();
    CurveLatDock->show();
}

void SectionWidgetForced::clearInterface() {
    GraphReplotTimer->stop();

    SectionSettingsDock->hide();
    SectionInfoDock->hide();
    CurveVertDock->hide();
    CurveRollDock->hide();
    CurveLatDock->hide();
}

void SectionWidgetForced::Update() {
    SectionForced->CurveRoll->replot();
    SectionForced->CurveVert->replot();
    SectionForced->CurveLat->replot();

    SectionWidget::Update();
}

void SectionWidgetForced::GraphReplotTimerDone() {
    if(!trackBuilder->graphChanges) return;
    SectionForced->CurveRoll->replot();
    SectionForced->CurveVert->replot();
    SectionForced->CurveLat->replot();
    trackBuilder->graphChanges = false;
}

SectionWidgetForced::~SectionWidgetForced()
{
    delete ui;
}

void SectionWidgetForced::on_useFixedSpeed_clicked(bool checked)
{
    SectionForced->bSpeed = checked?0:1;
    ui->inputSpeed->setEnabled(checked);

    Update();
}

void SectionWidgetForced::on_inputSpeed_valueChanged(double arg1)
{
    SectionForced->fVel = arg1;
    Update();
}

void SectionWidgetForced::on_showShowRollDerivative_clicked(bool checked)
{
    SectionForced->showShowRollDerivative = checked;
    SectionForced->updateGraphStates();
    SectionForced->CurveRoll->pointPlot->plotter->replot();
}

void SectionWidgetForced::on_showRoll_clicked(bool checked)
{
    SectionForced->showRoll = checked;
    SectionForced->updateGraphStates();
    SectionForced->CurveRoll->pointPlot->plotter->replot();
}
