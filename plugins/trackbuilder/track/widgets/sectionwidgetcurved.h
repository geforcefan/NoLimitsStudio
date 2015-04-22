#ifndef SECTIONWIDGETCURVED_H
#define SECTIONWIDGETCURVED_H

#include <QWidget>

#include "trackbuilder.h"
#include "sectionwidget.h"

class seccurved;

namespace Ui {
class SectionWidgetCurved;
}

class SectionWidgetCurved : public QWidget, public SectionWidget
{
    Q_OBJECT
    
public:
    explicit SectionWidgetCurved(section *_Section, QWidget *parent = 0);
    ~SectionWidgetCurved();

    void initInterface();
    void clearInterface();
    void Update();

    QDockWidget *SectionSettingsDock;
    QDockWidget *CurveRollDock;
private:
    Ui::SectionWidgetCurved *ui;
    seccurved *SectionCurved;
    QTimer *GraphReplotTimer;
public slots:
    void valuesChanged();
    void GraphReplotTimerDone();
private slots:
    void on_inputHeartline_valueChanged(double arg1);
    void on_inputDirection_valueChanged(double arg1);
    void on_inputLeadIn_valueChanged(double arg1);
    void on_inputLeadOut_valueChanged(double arg1);
    void on_showRoll_clicked(bool checked);
    void on_showShowRollDerivative_clicked(bool checked);
    void on_showVerticalForce_clicked(bool checked);
    void on_showLateralForce_clicked(bool checked);
    void on_useFixedSpeed_clicked(bool checked);
    void on_inputSpeed_valueChanged(double arg1);
};

#endif // SECTIONWIDGETCURVED_H
