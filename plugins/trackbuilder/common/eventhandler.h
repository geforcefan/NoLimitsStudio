#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <QCursor>
#include <QObject>

class EventHandler : public QObject
{
    Q_OBJECT
public:
    explicit EventHandler(QObject *parent = 0);
    void receiveEvent(QObject *obj, QEvent *event);
    
    bool usePov;
    QCursor *pCursor;
signals:
    void windowResize();
    
public slots:
    
};
#endif
