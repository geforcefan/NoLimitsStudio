#ifndef QFVDCURVE_H
#define QFVDCURVE_H

#include <QWidget>
#include <QDebug>
#include <QMutex>

#include "transition/transitionmanager.h"

namespace Ui {
class qFVDCurve;
}

class qPointPlot;

class qFVDCurvePoint
{
public:
    qFVDCurvePoint() {}
    qFVDCurvePoint(float _x, float _y) : x(_x), y(_y), refPoint(-1), transition(new TransitionManager()) {}
    qFVDCurvePoint(float _x, float _y, int _refPoint) : x(_x), y(_y), refPoint(_refPoint), transition(new TransitionManager()) {}
    float x,y;
    int refPoint;
    TransitionManager *transition;
};

class qFVDCurve : public QWidget
{
    Q_OBJECT
    
public:
    explicit qFVDCurve(QWidget *parent = 0);
    void setYLabel(QString label);
    void reinit();
    void replot();
    void setXRange(float from, float to);
    void setLengthRange(float from, float to);
    void setTimeRange(float from, float to);
    void initSettings();
    void initSettings(int i);
    void setResize();
    void clearPoints();
    void addPoint(qFVDCurvePoint *point);
    void modifyPoint(int index, qFVDCurvePoint point, bool bReplot = true);
    void resizeEvent( QResizeEvent * _e );
    void init();
    void setColor(QColor _color);
    void setYRange(float from, float to);
    void timeLengthChangeable(bool state);
    void enableLength();
    void enableTime();

    qFVDCurvePoint *getPoint(float x);
    float getQuaternionDerivative(float x);
    float getEulerY(float x);

    bool eventFilter(QObject *obj, QEvent *event);

    ~qFVDCurve();

    qPointPlot *pointPlot;

    std::vector <qFVDCurvePoint*> points;

    bool useTime, useLength;

    float timeRangeFrom;
    float timeRangeTo;

    float lengthRangeFrom;
    float lengthRangeTo;

public slots:

    void on_inputTimeRange_valueChanged(double arg1);

    void on_xValue_valueChanged(double arg1);

    void on_yValue_valueChanged(double arg1);

    void on_addNonLinear_clicked();

    void on_useLength_clicked(bool checked);

    void on_useTime_clicked(bool checked);

    void on_inputLengthRange_valueChanged(double arg1);
private:
    Ui::qFVDCurve *ui;

    int currentPoint;

    QColor color;

    QString labelX, labelY;

public slots:
    void pointSelected(int i);
    void pointMoving(int i);
    void pointDelete(int i);
    void pointRelease(int i);
    void windowResize();

signals:
    void valuesChanged();
private slots:
    void on_testX_valueChanged(double arg1);
    void on_addCombinedPoint_clicked();
};

#endif // QFVDCURVE_H
