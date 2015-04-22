#ifndef QPOINTPLOT_H
#define QPOINTPLOT_H

#include <QWidget>
#include <QGraphicsPixmapItem>
#include <qWidgets/qcustomplot.h>
#include <common/global.h>

#include <QMutex>

namespace Ui {
class qPointPlot;
}

class QGraphicsScene;
class qPointPlotPoint;
class qPointPlot;

class pointPixmapItem : public QGraphicsPixmapItem
{
public:
  pointPixmapItem( const QPixmap & pixmap) : QGraphicsPixmapItem(pixmap) { isMoving = false; }
  void setParentPoint(qPointPlotPoint *_point);
  void setGraphicsScene(qPointPlot *_pointPlot);
protected:
  void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
private:
  qPointPlotPoint *point;
  qPointPlot *pointPlot;

  bool isMoving;

  float prevX, prevY;
};

class qPointPlotPoint {
public:
    qPointPlotPoint() : x(0.0f), y(0.0f), index(0) {}
    qPointPlotPoint(float _x, float _y, int _i, pointPixmapItem *_pixmap) : x(_x), y(_y), index(_i), pixmap(_pixmap) {}
    float x, y;
    int index;
    pointPixmapItem *pixmap;
};

class qPointPlot : public QWidget
{
    Q_OBJECT
    
public:
    explicit qPointPlot(QWidget *parent = 0);
    ~qPointPlot();

    void init();
    void resizeStuff();
    void addPoint(float x, float y, int i);
    void reinitPoints();
    void doPointSelected(int i);
    void doPointRelease(int i);
    void doPointMoving(int i);
    void doPointDelete(int i);
    void deletePoint(int i);
    void setWidth(int width);
    void setScaleFactor(float f);

    std::vector <qPointPlotPoint*> points;
    QGraphicsScene* pointScene;

    QSize toRealCoord(float x, float y);
    glm::vec3 toPlotCoord(float x, float y);

    float xRangeFrom, xRangeTo;
    float yRangeFrom, yRangeTo;

    float scaleFactor;
    float originalScaleFactor;
    int widgetLength;
    int screenWidth;

    QCustomPlot *plotter;

    QPixmap pointFocused;
    QPixmap pointNotFocused;

    QColor color;

    bool xPressed;
    bool yPressed;

    bool fitToWidget;

    QMutex *Mutex;
protected:
    void resizeEvent(QResizeEvent *e);
    
private:
    Ui::qPointPlot *ui;

public slots:
    void mousePressEvent(QMouseEvent *);

signals:
    void pointSelected(int i);
    void pointMoving(int i);
    void pointDelete(int i);
    void pointRelease(int i);
};

#endif // QPOINTPLOT_H
