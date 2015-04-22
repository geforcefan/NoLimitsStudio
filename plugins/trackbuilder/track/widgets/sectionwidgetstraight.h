#ifndef SECTIONWIDGETSTRAIGHT_H
#define SECTIONWIDGETSTRAIGHT_H

#include <QWidget>
#include <QDockWidget>

#include "trackbuilder.h"
#include "sectionwidget.h"

class secstraight;
class qFVDCurve;

namespace Ui {
class SectionWidgetStraight;
}

class SectionWidgetStraight : public QWidget, public SectionWidget
{
    Q_OBJECT
    
public:
    explicit SectionWidgetStraight(section *_Section, QWidget *parent = 0);
    ~SectionWidgetStraight();

    void initInterface();
    void clearInterface();
    void Update();
    void test();

    QDockWidget *SectionSettingsDock;
    QDockWidget *CurveRollDock;
    
private:
    Ui::SectionWidgetStraight *ui;
    secstraight *SectionStraight;
    QTimer *GraphReplotTimer;

public slots:
    void valuesChanged();
    void GraphReplotTimerDone();

private slots:
    void on_useFixedSpeed_clicked(bool checked);
    void on_inputSpeed_valueChanged(double arg1);
    void on_useStartSpeed_clicked(bool checked);
    void on_inputStartSpeed_valueChanged(double arg1);
    void on_showRoll_clicked(bool checked);
    void on_showShowRollDerivative_clicked(bool checked);
    void on_showVerticalForce_clicked(bool checked);
    void on_showLateralForce_clicked(bool checked);
};

#endif // SECTIONWIDGETSTRAIGHT_H
