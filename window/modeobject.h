#ifndef MODEOBJECT_H
#define MODEOBJECT_H

#include <QObject>

class QAction;

class modeObject : public QObject
{
    Q_OBJECT
public:
    explicit modeObject(QObject *parent = 0);

    QAction *action;
    QString pluginIdentiefier;
    
signals:
    
public slots:
    
};

#endif // MODEOBJECT_H
