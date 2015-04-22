#ifndef SECTIONWIDGET_H
#define SECTIONWIDGET_H

#include <QDockWidget>
#include "sectionwidgetinfo.h"
#include "trackbuilder.h"

class section;
class SectionInfoCallback;

class SectionWidget
{
public:
    SectionWidget(section *_Section);

    virtual void initInterface() = 0;
    virtual void clearInterface() = 0;

    void Update(bool fromCurrentSection = true);

    SectionWidgetInfo *InfoWidget;
    section *Section;

    QDockWidget *SectionInfoDock;
private:
    SectionInfoCallback *sectionInfoCallback;
};

#endif // SECTIONWIDGET_H
