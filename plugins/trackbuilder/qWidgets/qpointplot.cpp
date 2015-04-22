#include "qPointPlot.h"
#include "ui_qpointplot.h"

#include <trackbuilder.h>

#include <QLayout>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPixmapItem>
#include <QGraphicsWidget>

#include <common/eventhandler.h>

void pointPixmapItem::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
  QGraphicsPixmapItem::mouseMoveEvent(event);

  if(!isMoving) {
      prevX = this->x();
      prevY = this->y();
  }
  isMoving = true;

  if(pointPlot->xPressed) this->setY(prevY);
  if(pointPlot->yPressed) this->setX(prevX);

  if(!pointPlot->yPressed) {
      if(this->x() < 0) this->setX(0);
      if(this->x() > pointPlot->pointScene->width() - 10) this->setX(pointPlot->pointScene->width() - 10);
  }

  if(!pointPlot->xPressed) {
      if(this->y() < 1) this->setY(1);
      if(this->y() > pointPlot->pointScene->height() - 9) this->setY(pointPlot->pointScene->height() - 9);
  }

  qPointPlotPoint *nextPoint, *prevPoint;

  if(!pointPlot->yPressed) {
      prevPoint = pointPlot->points[point->index-1];
      if(point->index != pointPlot->points.size()-1 ) {
          nextPoint = pointPlot->points[point->index+1];
          if(this->x() > nextPoint->pixmap->x()) this->setX(nextPoint->pixmap->x());
      }
      if(this->x() < prevPoint->pixmap->x()) this->setX(prevPoint->pixmap->x());
  }

  glm::vec3 points;

  if(!pointPlot->yPressed) points = pointPlot->toPlotCoord(this->x(), this->y()-1);
  else points = pointPlot->toPlotCoord(this->x(), this->y()-1);

  point->x = points.x;
  point->y = points.y;

  pointPlot->reinitPoints();
  pointPlot->doPointMoving(point->index);
}

void pointPixmapItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsPixmapItem::mouseReleaseEvent(event);

    if(!isMoving) {
        if(event->button() == Qt::RightButton) {
            if(point->index && point->index == pointPlot->points.size()-1 ) {
                pointPlot->pointScene->removeItem(this);
                pointPlot->points.erase(pointPlot->points.begin() + point->index);
                pointPlot->doPointDelete(point->index);
            }
        }
        if(event->button() == Qt::LeftButton) {

            for(int i=0; i < pointPlot->points.size(); i++) {
                pointPlot->points[i]->pixmap->setPixmap(pointPlot->pointNotFocused);
            }

            this->setPixmap(pointPlot->pointFocused);
            pointPlot->doPointSelected(point->index);
        }
    }
    isMoving = false;

    pointPlot->doPointRelease(point->index);
}

void pointPixmapItem::setParentPoint(qPointPlotPoint *_point) {
    point = _point;
}
void pointPixmapItem::setGraphicsScene(qPointPlot *_pointPlot) {
    pointPlot = _pointPlot;
}

qPointPlot::qPointPlot(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::qPointPlot)
{
    Mutex = new QMutex();
    ui->setupUi(this);

    setScaleFactor(25);
    widgetLength = 0;
    fitToWidget = true;

    color = QColor(255, 255, 0, 255);

    pointScene = new QGraphicsScene(this);
    plotter = new QCustomPlot(this);

    QObject::connect(plotter, SIGNAL(clicked()),
                     this, SLOT(plotterClicked()));

    xPressed = yPressed = false;

    ui->plotWidget = plotter;
    ui->pointsArea->raise();
    ui->pointsArea->setStyleSheet("background: transparent");
    ui->pointsArea->setScene(pointScene);
}

void qPointPlot::setScaleFactor(float f) {
    scaleFactor = f;
    originalScaleFactor = f;
}

void qPointPlot::mousePressEvent(QMouseEvent *) {
    emit pointSelected(-1);
    for(int i=0; i < points.size(); i++) {
        points[i]->pixmap->setPixmap(pointNotFocused);
    }
}

void qPointPlot::init() {    // Points graphics init
    pointNotFocused = QPixmap(10,10);
    QPainter p(&pointNotFocused);
    p.setPen(QPen(color, 2)); // line color blue for first graph
    p.setBrush(QBrush(QColor(255, 255, 255, 255))); // first graph will be filled with translucent blue
    p.drawRect(0,0,10,10);

    pointFocused = QPixmap(10,10);
    QPainter p2(&pointFocused);
    p2.setPen(QPen(color, 2)); // line color blue for first graph
    p2.setBrush(QBrush(color)); // first graph will be filled with translucent blue
    p2.drawRect(0,0,10,10);

    plotter->addGraph();
    plotter->setAutoMargin(false);
    plotter->setMargin(50, 50, 50, 50);
    plotter->graph(0)->setPen(QPen(color)); // line color blue for first graph
    plotter->graph(0)->setBrush(QBrush(QColor(color.red(), color.green(), color.blue(), 20))); // first graph will be filled with translucent blue

    plotter->xAxis->setRange(xRangeFrom, xRangeTo);
    plotter->yAxis->setRange(yRangeFrom, yRangeTo);
    plotter->replot();

    resizeStuff();
}

void qPointPlot::deletePoint(int i) {
    pointScene->removeItem(points[i]->pixmap);
    points.erase(points.begin() + i);
}

void qPointPlot::addPoint(float x, float y, int i) {
    pointPixmapItem* item = new pointPixmapItem(pointNotFocused);

    item->setGraphicsScene(this);
    item->setX(0);
    item->setY(0);
    if(points.size()) item->setFlag(QGraphicsItem::ItemIsMovable);
    pointScene->addItem(item);

    points.push_back(new qPointPlotPoint(x, y, i, item));
    points[points.size()-1]->pixmap->setParentPoint(points[points.size()-1]);
}
void qPointPlot::doPointRelease(int i) {
    emit pointRelease(i);
}
void qPointPlot::doPointSelected(int i) {
    emit pointSelected(i);
}
void qPointPlot::doPointMoving(int i) {
    emit pointMoving(i);
}
void qPointPlot::doPointDelete(int i) {
    emit pointDelete(i);
}

void qPointPlot::setWidth(int width) {
    screenWidth = width;
    resizeStuff();
}

void qPointPlot::resizeStuff() {
    widgetLength = (fitToWidget) ? screenWidth : xRangeTo * scaleFactor;

    plotter->setGeometry(QRect(0,0,widgetLength,height()));
    ui->pointsArea->setGeometry(QRect(45,45,widgetLength-90,height()-90));

    pointScene->setSceneRect(0,0,widgetLength-90, height()-90);

    this->setGeometry(QRect(0,0,widgetLength,height()));
    this->setMinimumWidth(widgetLength-10);
    reinitPoints();
}

void qPointPlot::resizeEvent(QResizeEvent *e) {
    resizeStuff();
}

void qPointPlot::reinitPoints() {
    for(int i=0; i < points.size(); i++) {
        QSize newcoord = toRealCoord(points[i]->x, points[i]->y);
        points[i]->pixmap->setX(newcoord.width());
        points[i]->pixmap->setY(newcoord.height());
    }
}

QSize qPointPlot::toRealCoord(float x, float y) {
    QSize out;

    float xrangeReal = widgetLength - 100;
    float yrangeReal = height() - 100;

    float xrangePlot = xRangeTo - xRangeFrom;
    float yrangePlot = yRangeTo - yRangeFrom;

    out.setWidth(((x-xRangeFrom)/xrangePlot) * xrangeReal);
    out.setHeight(yrangeReal - (((y-yRangeFrom)/yrangePlot) * yrangeReal));

    return out;
}

glm::vec3 qPointPlot::toPlotCoord(float x, float y) {
    glm::vec3 out;

    float xrangeReal = widgetLength - 100;
    float yrangeReal = height() - 100;

    float xrangePlot = xRangeTo - xRangeFrom;
    float yrangePlot = yRangeTo - yRangeFrom;

    out.x = (((x)/xrangeReal) * xrangePlot) + xRangeFrom;
    out.y = ( ((yrangeReal-y)/yrangeReal) * yrangePlot) + yRangeFrom;

    return out;
}

qPointPlot::~qPointPlot()
{
    delete ui;
}
