#ifndef SECTIONWIDGETFORCED_H
#define SECTIONWIDGETFORCED_H

#include <QWidget>

#include "trackbuilder.h"
#include "sectionwidget.h"

class secforced;

namespace Ui {
class SectionWidgetForced;
}

class SectionWidgetForced : public QWidget, public SectionWidget
{
    Q_OBJECT

public:
    explicit SectionWidgetForced(section *_Section, QWidget *parent = 0);
    ~SectionWidgetForced();

    void initInterface();
    void clearInterface();
    void Update();

    QDockWidget *SectionSettingsDock;
    QDockWidget *CurveVertDock;
    QDockWidget *CurveRollDock;
    QDockWidget *CurveLatDock;
private:
    Ui::SectionWidgetForced *ui;
    secforced *SectionForced;
    QTimer *GraphReplotTimer;
public slots:
    void valuesChanged();
    void GraphReplotTimerDone();
private slots:
    void on_showRoll_clicked(bool checked);
    void on_showShowRollDerivative_clicked(bool checked);
    void on_useFixedSpeed_clicked(bool checked);
    void on_inputSpeed_valueChanged(double arg1);
};

#endif // SECTIONWIDGETFORCED_no
