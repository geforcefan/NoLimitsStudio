#ifndef SECTIONWIDGETINFO_H
#define SECTIONWIDGETINFO_H

#include <QWidget>

namespace Ui {
class SectionWidgetInfo;
}

class section;
class SectionWidget;

class SectionWidgetInfo : public QWidget
{
    Q_OBJECT
    
public:
    explicit SectionWidgetInfo(section *_Section, SectionWidget *_parentSectionWidget, QWidget *parent = 0);
    ~SectionWidgetInfo();
    void UpdateValues();
    Ui::SectionWidgetInfo *ui;

    section *Section;
    SectionWidget *parentSectionWidget;
private slots:
    void on_inputPosX_valueChanged(double arg1);

    void on_inputPosY_valueChanged(double arg1);

    void on_inputPosZ_valueChanged(double arg1);

    void on_inputPitch_valueChanged(double arg1);

    void on_inputYaw_valueChanged(double arg1);

    void on_inputNormal_valueChanged(double arg1);

    void on_inputLateral_valueChanged(double arg1);

    void on_inputSpeed_valueChanged(double arg1);

    void on_inputHeartline_valueChanged(double arg1);

private:
};


#endif // SECTIONWIDGETINFO_H
