#include <QSpinBox>
#include <QLayout>

#include "qfvdcurve.h"
#include "ui_qfvdcurve.h"

#include <qWidgets/qpointplot.h>
#include <QLayout>

#include <trackbuilder.h>

#include <QEvent>
#include <common/eventhandler.h>

#include <QWheelEvent>
#include <transition/transitionmanagerwidget.h>

qFVDCurve::qFVDCurve(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::qFVDCurve)
{
    ui->setupUi(this);

    this->installEventFilter(this);
    this->setMouseTracking(true);

    pointPlot = new qPointPlot();

    QObject::connect(pointPlot, SIGNAL(pointSelected(int)),
                     this, SLOT(pointSelected(int)));
    QObject::connect(pointPlot, SIGNAL(pointMoving(int)),
                     this, SLOT(pointMoving(int)));
    QObject::connect(pointPlot, SIGNAL(pointDelete(int)),
                     this, SLOT(pointDelete(int)));
    QObject::connect(pointPlot, SIGNAL(pointRelease(int)),
                     this, SLOT(pointRelease(int)));
    QObject::connect(trackBuilder->Events, SIGNAL(windowResize()),
                     this, SLOT(windowResize()));

    pointPlot->setMinimumHeight(200);

    ui->settings->hide();
    timeLengthChangeable(false);

    this->ui->scrollAreaWidgetContents->layout()->addWidget(pointPlot);
    this->setMinimumHeight(270);

    color = QColor(255, 0, 0, 255);

    ui->coordinates->setText("");

    // init values
    lengthRangeFrom = 0.0f;
    lengthRangeTo = 20.0f;

    timeRangeFrom = 0.0f;
    timeRangeTo = 2.0f;

    pointPlot->xRangeFrom = 0;
    pointPlot->xRangeTo = 20;

    pointPlot->yRangeFrom = -10;
    pointPlot->yRangeTo = 10;
}

bool qFVDCurve::eventFilter(QObject *obj, QEvent *event) {

    // Update Sizes
    if( event->type() == QEvent::Show ||
        event->type() == QEvent::UpdateLater ||
        event->type() == QEvent::Resize ||
        event->type() == QEvent::LayoutRequest ||
        event->type() == QEvent::Paint ||
        event->type() == QEvent::Enter ||
        event->type() == QEvent::Leave ) {
        setResize();
    }

    // Press Key event
    if(event->type() == 51) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if(!keyEvent->isAutoRepeat()) {
            if(keyEvent->key() == Qt::Key_X) pointPlot->xPressed = true;
            if(keyEvent->key() == Qt::Key_Y) pointPlot->yPressed = true;
            if(keyEvent->key() == Qt::Key_R) {
                pointPlot->fitToWidget = true;
                pointPlot->resizeStuff();
            }
        }
    }

    // Release Key event
    if(event->type() == 7) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if(!keyEvent->isAutoRepeat()) {
            if(keyEvent->key() == Qt::Key_X) pointPlot->xPressed = false;
            if(keyEvent->key() == Qt::Key_Y) pointPlot->yPressed = false;
        }
    }

    if(event->type() == QEvent::Wheel) {
        QWheelEvent *wheelEvent = static_cast<QWheelEvent *>(event);

        if(pointPlot->fitToWidget) {
            pointPlot->setScaleFactor(pointPlot->widgetLength / pointPlot->xRangeTo);
            pointPlot->fitToWidget = false;
        }

        float factor = 20/pointPlot->xRangeTo;

        if(wheelEvent->delta() < 0) {
            pointPlot->scaleFactor += factor;
        } else {
            pointPlot->scaleFactor -= factor;
        }

        if(pointPlot->scaleFactor * pointPlot->xRangeTo < 250) pointPlot->scaleFactor += factor;

        pointPlot->resizeStuff();
    }
}

void qFVDCurve::timeLengthChangeable(bool state) {
    if(state) ui->independentSettings->show();
    else ui->independentSettings->hide();
}

void qFVDCurve::enableLength() {
    useLength = true;
    useTime = false;

    ui->useLength->setChecked(true);
    labelX = QString("Length");
    reinit();

    emit valuesChanged();
}

void qFVDCurve::enableTime() {
    useLength = false;
    useTime = true;

    ui->useTime->setChecked(true);
    labelX = QString("Time");
    reinit();

    emit valuesChanged();
}

void qFVDCurve::setYRange(float from, float to) {
    pointPlot->yRangeFrom = from;
    pointPlot->yRangeTo = to;

    ui->yValue->setMaximum(to);
    ui->yValue->setMinimum(from);

}

void qFVDCurve::setColor(QColor _color) {
    color = QColor(_color);
}

void qFVDCurve::init() {
    pointPlot->color = QColor(color);

    pointPlot->init();
    reinit();
    setResize();
    pointPlot->reinitPoints();
}

void qFVDCurve::addPoint(qFVDCurvePoint *point) {
    int i=points.size();
    points.push_back(point);
    pointPlot->addPoint(points[i]->x, points[i]->y, i);
}

void qFVDCurve::clearPoints() {
    for(int i=points.size()-1; i > 0; i--) {
        pointDelete(i);
    }
}

void qFVDCurve::modifyPoint(int index, qFVDCurvePoint point, bool bReplot) {
    points[index]->x = point.x;
    points[index]->y = point.y;
    points[index]->refPoint = point.refPoint;
    points[index]->transition = point.transition;

    pointPlot->points[index]->x = point.x;
    pointPlot->points[index]->y = point.y;

    pointPlot->reinitPoints();
    if(bReplot) replot();
}

void qFVDCurve::setResize() {
    pointPlot->setWidth(ui->scrollArea->width());

    ui->layoutWidget->setGeometry(QRect(0,0,width(), height()));
    ui->addPointsWidget->raise();
    ui->addPointsWidget->setGeometry(QRect(0,40,width()-10-120,40));
    ui->informationWidget->setGeometry(QRect(0,height()-55,width()-10-190,40));
}

void qFVDCurve::windowResize() {
    setResize();
}

void qFVDCurve::resizeEvent( QResizeEvent * _e ) {
    setResize();
}

void qFVDCurve::pointSelected(int i) {
    if(i < 0) {
        ui->settings->hide();
        setResize();
        return;
    }
    currentPoint = i;
    ui->settings->show();

    if ( ui->TransitionPanel->layout() != NULL )
    {
        QLayoutItem* item;
        while ( ( item = ui->TransitionPanel->layout()->takeAt( 0 ) ) != NULL )
        {
            delete item->widget();
            delete item;
        }
    }

    TransitionManagerWidget *transitionManagerWidget = new TransitionManagerWidget(points[currentPoint]->transition);
    connect(transitionManagerWidget, SIGNAL(valuesChanged()), SIGNAL(valuesChanged()));
    ui->TransitionPanel->layout()->addWidget(static_cast<QWidget*>(transitionManagerWidget));

    initSettings();
    setResize();
}

void qFVDCurve::pointMoving(int i) {
    points[i]->x = pointPlot->points[i]->x;
    points[i]->y = pointPlot->points[i]->y;
    replot();
    if(i == currentPoint) initSettings(i);

    ui->coordinates->setText(QString("").sprintf("%s: %2.3f, %s: %2.3f", NoLimitsRenderer::toChar(labelX),  points[i]->x, NoLimitsRenderer::toChar(labelY),  points[i]->y));

    emit valuesChanged();
}

void qFVDCurve::pointRelease(int i) {
    ui->coordinates->setText("");
}

void qFVDCurve::pointDelete(int i) {
    if(currentPoint == i) currentPoint = 0;

    if(points[i]->refPoint >= 0) {
        points.erase(points.begin() + i);
        points.erase(points.begin() + points[i]->refPoint);
        pointPlot->deletePoint(points[i]->refPoint);
    } else {
        points.erase(points.begin() + i);
        //pointPlot->deletePoint(i);
    }
    replot();
    ui->settings->hide();

    emit valuesChanged();
}

void qFVDCurve::setXRange(float from, float to) {
    pointPlot->xRangeFrom = from;
    pointPlot->xRangeTo = to;
}

void qFVDCurve::setLengthRange(float from, float to) {
    lengthRangeFrom = from;
    lengthRangeTo = to;
}

void qFVDCurve::setTimeRange(float from, float to) {
    timeRangeFrom = from;
    timeRangeTo = to;
}

void qFVDCurve::replot() {
    pointPlot->Mutex->lock();
    if(points.size() < 2) {
        pointPlot->plotter->graph(0)->clearData();
        pointPlot->plotter->replot();
    }

    if(ui->useLength->isChecked()) {
        if(points[points.size()-1]->x < pointPlot->xRangeTo-0.0000001f) {
            //ui->information->setText("Last point has to be match with length");
            ui->addPointsWidget->show();
        }
        else {
            ui->addPointsWidget->hide();
            //ui->information->setText("");
        }
    } else {
        ui->addPointsWidget->show();
        //ui->information->setText("");
    }

    std::vector<glm::vec2> plotPoints;
    qFVDCurvePoint *firstPoint = points[0];

    for(int i=1; i < points.size(); i++) {
        qFVDCurvePoint *currentPoint = points[i];
        qFVDCurvePoint *prevPoint = points[i-1];

        float lastY = 0;
        float lastX = 0;
        float changeX = 0;
        float changeY = 0;

        if(i==1) {
            lastY = firstPoint->y;
            lastX = firstPoint->x;
        } else {
            lastX = prevPoint->x;
            lastY = prevPoint->transition->getLastY();
        }

        changeX = currentPoint->x - lastX;
        changeY = currentPoint->y - lastY;

        currentPoint->transition->setChangeX(changeX);
        currentPoint->transition->setChangeY(changeY);
        currentPoint->transition->setStartX(lastX);
        currentPoint->transition->setStartY(lastY);


        float stepSize = changeX / 150;
        for(float t=(prevPoint->x); t <= currentPoint->x; t += stepSize) {
            plotPoints.push_back(glm::vec2(t, currentPoint->transition->getY(t)));
        }
    }

    QVector<double> x(plotPoints.size()), y(plotPoints.size());
    for(int i=0; i < plotPoints.size(); i++) { x[i] = plotPoints[i].x; y[i] = plotPoints[i].y; }

    pointPlot->plotter->graph(0)->setData(x, y);
    pointPlot->plotter->replot();
    pointPlot->Mutex->unlock();
}

void qFVDCurve::reinit() {
    if(ui->useLength->isChecked()) {
        ui->timeRange->hide();
        ui->lengthRange->show();
        this->ui->labelX->setText(QString("Length value"));
        setXRange(lengthRangeFrom, lengthRangeTo);
    } else {
        ui->timeRange->show();
        ui->lengthRange->hide();
        this->ui->labelX->setText(QString("Time value"));
        ui->inputTimeRange->setValue((double)timeRangeTo);
        setXRange(timeRangeFrom, timeRangeTo);
    }

    pointPlot->Mutex->lock();
    pointPlot->plotter->xAxis->setRange(pointPlot->xRangeFrom, pointPlot->xRangeTo);
    pointPlot->plotter->yAxis->setRange(pointPlot->yRangeFrom, pointPlot->yRangeTo);
    pointPlot->resizeStuff();
    pointPlot->Mutex->unlock();

    replot();
}

void qFVDCurve::initSettings() {
    initSettings(currentPoint);
}

void qFVDCurve::initSettings(int i) {
    if(!i) {
        ui->xValue->setEnabled(false);
        ui->yValue->setEnabled(false);
    } else {
        ui->xValue->setEnabled(true);
        ui->yValue->setEnabled(true);
    }

    ui->xValue->blockSignals(true);
    ui->xValue->setValue(points[i]->x);
    ui->xValue->blockSignals(false);

    ui->yValue->blockSignals(true);
    ui->yValue->setValue(points[i]->y);
    ui->yValue->blockSignals(false);

    ui->coordinates->blockSignals(true);
    ui->coordinates->setText("");
    ui->coordinates->blockSignals(false);
}

void qFVDCurve::setYLabel(QString label) {
    labelY = QString(label);
    this->ui->labelY->setText(QString(label).append(" value"));
}

qFVDCurve::~qFVDCurve()
{
    delete ui;
}

void qFVDCurve::on_inputTimeRange_valueChanged(double arg1)
{
    if(NoLimitsRenderer::isZero((float)arg1)) arg1 = 2.0;

    setTimeRange(0.0f, (float)arg1);
    reinit();

    emit valuesChanged();
}

void qFVDCurve::on_inputLengthRange_valueChanged(double arg1)
{
    setLengthRange(0.0f, (float)arg1);
    reinit();

    emit valuesChanged();
}

void qFVDCurve::on_xValue_valueChanged(double arg1)
{
    pointPlot->points[currentPoint]->x = points[currentPoint]->x = arg1;
    pointPlot->reinitPoints();
    replot();

    emit valuesChanged();
}

void qFVDCurve::on_yValue_valueChanged(double arg1)
{
    pointPlot->points[currentPoint]->y = points[currentPoint]->y = arg1;
    pointPlot->reinitPoints();
    replot();

    emit valuesChanged();
}

void qFVDCurve::on_addNonLinear_clicked()
{
    float newX = points[points.size()-1]->x + ((pointPlot->xRangeTo - points[points.size()-1]->x) / 2);
    float newY = points.size()-1==0?points[points.size()-1]->y:points[points.size()-1]->transition->getY(points[points.size()-1]->x);
    if(ui->useLength->isChecked()) newX = pointPlot->xRangeTo;


    //qDebug() << "pointPlot->xRangeTo" << pointPlot->xRangeTo << "newX" << newX << "newY" << newY;

    addPoint(new qFVDCurvePoint(newX, newY));

    TransitionManager *newTransition = points[points.size()-1]->transition;
    TransitionManager *lastTransition = points[points.size()-2]->transition;

    if(points.size()-2 != 0) {
        newTransition->setPrevTransition(lastTransition);
        lastTransition->setNextTransition(newTransition);
    }

    pointPlot->reinitPoints();
    replot();

    emit valuesChanged();
}

void qFVDCurve::on_addCombinedPoint_clicked()
{

    float newX2 = points[points.size()-1]->x + ((pointPlot->xRangeTo - points[points.size()-1]->x) / 2);
    float newY2 = points.size()-1==0?points[points.size()-1]->y:points[points.size()-1]->transition->getY(points[points.size()-1]->x);
    if(ui->useLength->isChecked()) newX2 = pointPlot->xRangeTo;

    float newX1 = points[points.size()-1]->x + ((newX2 - points[points.size()-1]->x) / 2);
    float newY1 = newY2;

    addPoint(new qFVDCurvePoint(newX1, newY1));
    TransitionManager *newTransition = points[points.size()-1]->transition;
    TransitionManager *lastTransition = points[points.size()-2]->transition;
    newTransition->setTransitionType(TransitionManager::GenericTransition);
    newTransition->setPrevTransition(lastTransition);
    lastTransition->setNextTransition(newTransition);

    addPoint(new qFVDCurvePoint(newX2, newY2, points.size()-1));
    newTransition = points[points.size()-1]->transition;
    lastTransition = points[points.size()-2]->transition;
    newTransition->setTransitionType(TransitionManager::LinearTransition);
    newTransition->setPrevTransition(lastTransition);
    lastTransition->setNextTransition(newTransition);

    pointPlot->reinitPoints();
    replot();

    emit valuesChanged();
}


/* Calculation */
qFVDCurvePoint *qFVDCurve::getPoint(float x) {
    qFVDCurvePoint *point = points[0];
    if(x > points[points.size()-1]->x) return points[points.size()-1];
    if(NoLimitsRenderer::isZero(x) || x < 0.0f) return points[1];

    for(int i = 0; i < points.size(); i++) {
        if(points[i]->x >= x) {
            point = points[i];
            break;
        }
    }

    return point;
}

float qFVDCurve::getQuaternionDerivative(float x) {
    if(points.size() < 2) return 0.0f;
    qFVDCurvePoint *point = getPoint(x);

    return point->transition->getDerivative(x);
}

float qFVDCurve::getEulerY(float x) {
    if(points.size() < 2) return 0.0f;
    qFVDCurvePoint *point = getPoint(x);

    return point->transition->getY(x);
}

void qFVDCurve::on_useLength_clicked(bool checked)
{
    enableLength();
}

void qFVDCurve::on_useTime_clicked(bool checked)
{
    enableTime();
}

void qFVDCurve::on_testX_valueChanged(double arg1)
{
    getPoint((float)arg1);
}
