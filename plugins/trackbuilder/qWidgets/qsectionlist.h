#ifndef QSECTIONLIST_H
#define QSECTIONLIST_H

#include <QWidget>
#include <QStandardItemModel>
#include <common/global.h>

#include "track/section.h"

class SectionWidget;

namespace Ui {
class qSectionList;
}

class TrackBuilder;
class QTreeWidgetItem;
class QSignalMapper;

class SectionContextObject : public QObject
{
    Q_OBJECT
public:
    explicit SectionContextObject(secType _sectionType, QObject *parent = 0) : QObject(parent), sectionType(_sectionType) {}
    secType sectionType;
};

class qSectionList : public QWidget
{
    Q_OBJECT
    
public:
    explicit qSectionList(QWidget *parent = 0, TrackBuilder *_tb = 0);
    ~qSectionList();
    void addSection(QString SectionName);
    void clear();
    Ui::qSectionList *ui;

    void activateSection(int index);

private slots:
    void on_listView_clicked(const QModelIndex &index);
    void addSectionFromContext(QObject *modeObj);
    void on_listView_customContextMenuRequested(const QPoint &pos);

    void on_embedTrack_clicked();

private:
    TrackBuilder *tb;
    QStandardItemModel *model;
    SectionWidget *sectionWidget;

    int totalItems;
    QSignalMapper *contextMenuMapper;
};

#endif // QSECTIONLIST_H
