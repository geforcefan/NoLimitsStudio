#ifndef TRANSITIONMANAGERWIDGET_H
#define TRANSITIONMANAGERWIDGET_H

#include <QWidget>

class TransitionManager;
class TransitionGeneric;

namespace Ui {
class TransitionManagerWidget;
}

class TransitionManagerWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit TransitionManagerWidget(TransitionManager *_transition, QWidget *parent = 0);
    ~TransitionManagerWidget();
    void buildGui();
    void Update();

    double getShapeValue(int shape, double range);
    int getShapeValue(double shape, double range);
    
private:
    Ui::TransitionManagerWidget *ui;
    TransitionManager *transition;
    TransitionGeneric *tGeneric;

    double rangeSlope;
    double rangeShape1;
    double rangeShape2;

signals:
    void valuesChanged();
private slots:
    void on_sliderCenter_sliderMoved(int position);
    void on_sliderTension_sliderMoved(int position);
    void on_comboMode_currentIndexChanged(int index);
    void on_comboType_currentIndexChanged(int index);
    void on_useCustomSlope_clicked(bool checked);
    void on_inputSlope_valueChanged(double arg1);
    void on_inputShape1_valueChanged(double arg1);
    void on_inputShape2_valueChanged(double arg1);
    void on_sliderSlope_sliderMoved(int position);
    void on_sliderShape1_sliderMoved(int position);
    void on_sliderShape2_sliderMoved(int position);
};

#endif // TRANSITIONMANAGERWIDGET_H
