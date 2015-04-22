#include <QSpinBox>

#include "qsectionlist.h"
#include "ui_qsectionlist.h"

#include <trackbuilder.h>

#include <QSignalMapper>

#include <QDebug>

#include "track/track.h"

#include "track/secstraight.h"
#include "track/seccurved.h"
#include "track/secforced.h"
#include "track/widgets/sectionwidgetstraight.h"
#include "track/widgets/sectionwidgetcurved.h"
#include "track/widgets/sectionwidgetforced.h"

#include <maingl/glrenderer.h>
#include <maingl/glthread.h>
#include <maingl/glwidget.h>
#include <common/trackcamera.h>

qSectionList::qSectionList(QWidget *parent, TrackBuilder *_tb) :
    QWidget(parent),
    ui(new Ui::qSectionList)
{
    ui->setupUi(this);
    tb = _tb;

    sectionWidget = NULL;
    clear();

    installEventFilter(this);

    contextMenuMapper = new QSignalMapper(this);
    connect(contextMenuMapper, SIGNAL(mapped(QObject *)), this, SLOT(addSectionFromContext(QObject *)));

    connect(ui->embedTrack, SIGNAL(clicked()), this, SLOT(embedTrack()));
}

void qSectionList::clear() {
    totalItems = 0;

    model = new QStandardItemModel(0, 1, this);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Section"));

    ui->listView->setModel(model);
    ui->listView->setRootIsDecorated(false);
    ui->listView->setAlternatingRowColors(true);
}

void qSectionList::on_listView_clicked(const QModelIndex &index)
{
    activateSection(index.row());
}

void qSectionList::on_listView_customContextMenuRequested(const QPoint &pos)
{
    QMenu* contextMenu = new QMenu(this);

    Q_CHECK_PTR (contextMenu);

    QAction *ActionForced = contextMenu->addAction("New Forced Section", contextMenuMapper, SLOT(map()));
    contextMenuMapper->setMapping(ActionForced, new SectionContextObject(forced));

    QAction *ActionStraight = contextMenu->addAction("New Straight Section", contextMenuMapper, SLOT(map()));
    contextMenuMapper->setMapping(ActionStraight, new SectionContextObject(straight));

    QAction *ActionCurved = contextMenu->addAction("New Curved Section", contextMenuMapper, SLOT(map()));
    contextMenuMapper->setMapping(ActionCurved, new SectionContextObject(curved));

    contextMenu->popup(QCursor::pos());
    contextMenu->exec();


    delete contextMenu;
}

void qSectionList::addSection(QString SectionName) {
    model->insertRow(totalItems);
    model->setData(model->index(totalItems, 0), SectionName);
    totalItems++;
}

void qSectionList::addSectionFromContext(QObject *contextObject) {
    SectionContextObject *selectedSection = qobject_cast<SectionContextObject*>(contextObject);

    section *newSection = trackBuilder->BuilderTrack->newSection(selectedSection->sectionType);
    addSection(newSection->sName);

    activateSection(totalItems-1);
}

void qSectionList::activateSection(int index) {
    trackBuilder->rendererThread->start();

    trackBuilder->BuilderTrack->activeSection = trackBuilder->BuilderTrack->lSections.at(index);
    secType typ = trackBuilder->BuilderTrack->activeSection->type;

    if(sectionWidget != NULL) sectionWidget->clearInterface();
    if(typ == straight) {
        sectionWidget = new SectionWidgetStraight(static_cast<secstraight*>(trackBuilder->BuilderTrack->activeSection));
        sectionWidget->initInterface();
    }
    if(typ == curved) {
        sectionWidget = new SectionWidgetCurved(static_cast<seccurved*>(trackBuilder->BuilderTrack->activeSection));
        sectionWidget->initInterface();
    }
    if(typ == forced) {
        sectionWidget = new SectionWidgetForced(static_cast<secforced*>(trackBuilder->BuilderTrack->activeSection));
        sectionWidget->initInterface();
    }
}

qSectionList::~qSectionList()
{
    delete ui;
}

void qSectionList::on_embedTrack_clicked()
{
    qDebug() << "on_embedTrack_clicked";
}
