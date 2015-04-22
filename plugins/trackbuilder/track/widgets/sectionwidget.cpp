#include "sectionwidget.h"
#include "trackbuilder.h"

#include <QDebug>

class SectionInfoCallback: public TrackUpdateThreadCallback {
public:
    SectionInfoCallback(SectionWidget *_sectionWidget) : TrackUpdateThreadCallback() {
        sectionWidget = _sectionWidget;
    }
    void Execute() {
        sectionWidget->InfoWidget->UpdateValues();
    }
    SectionWidget *sectionWidget;
};

SectionWidget::SectionWidget(section *_Section)
{
    Section = _Section;
    sectionInfoCallback = new SectionInfoCallback(this);
    InfoWidget = new SectionWidgetInfo(Section, this);

    // Section Settings
    SectionInfoDock = new QDockWidget("Information panel", trackBuilder->mainWindow);
    SectionInfoDock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
    SectionInfoDock->setWidget(InfoWidget);
}

void SectionWidget::Update(bool fromCurrentSection) {
    trackBuilder->trackUpdateThread->setCallback(sectionInfoCallback);

    if(fromCurrentSection) trackBuilder->trackUpdateThread->setSection(Section);
    else trackBuilder->trackUpdateThread->hasSection = false;

    trackBuilder->trackUpdateThread->running = true;
}
