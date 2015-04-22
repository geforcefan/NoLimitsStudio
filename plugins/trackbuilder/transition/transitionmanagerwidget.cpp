#include "transitionmanagerwidget.h"
#include "ui_transitionmanagerwidget.h"
#include "transitionmanager.h"
#include "transitiongeneric.h"

#include <QDebug>

TransitionManagerWidget::TransitionManagerWidget(TransitionManager *_transition, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TransitionManagerWidget)
{
    ui->setupUi(this);
    transition = _transition;

    rangeSlope = 25.0f;
    rangeShape1 = 100.0f;
    rangeShape2 = 1500.0f;

    buildGui();
}

void TransitionManagerWidget::buildGui() {
    ui->comboMode->blockSignals(true);
    ui->comboMode->clear();
    std::vector<QString> modesList = transition->getModes();
    for(int i=0; i < modesList.size(); i++) ui->comboMode->addItem(modesList[i]);
    ui->comboMode->setCurrentIndex(transition->getMode());
    ui->comboMode->blockSignals(false);

    if(modesList.size()) {
        ui->comboMode->show();
        ui->labelMode->show();
    } else {
        ui->comboMode->hide();
        ui->labelMode->hide();
    }

    ui->comboType->blockSignals(true);
    if(transition->getTransitionType() == TransitionManager::BasicTransition) ui->comboType->setCurrentIndex(0);
    if(transition->getTransitionType() == TransitionManager::BumpTransition) ui->comboType->setCurrentIndex(1);
    if(transition->getTransitionType() == TransitionManager::LinearTransition) ui->comboType->setCurrentIndex(2);
    if(transition->getTransitionType() == TransitionManager::QuinticTransition) ui->comboType->setCurrentIndex(4);
    if(transition->getTransitionType() == TransitionManager::GenericTransition) ui->comboType->setCurrentIndex(5);
    ui->comboType->blockSignals(false);

    ui->sliderCenter->setValue((int)(transition->getCenter() / (1.0f/66.7f)));
    ui->sliderTension->setValue((int)(transition->getTension() / (1.0f/75.0f)));

    if(transition->getTransitionType() == TransitionManager::GenericTransition) {
        tGeneric = static_cast<TransitionGeneric*>(transition->getTransition());

        ui->sliderCenter->hide();
        ui->sliderTension->hide();
        ui->labelCenter->hide();
        ui->labelTension->hide();
        ui->transitionGeneric->show();

        ui->useCustomSlope->setChecked(tGeneric->useSlope);
        on_inputSlope_valueChanged(tGeneric->slope);
        on_inputShape1_valueChanged(tGeneric->shape1);
        on_inputShape2_valueChanged(tGeneric->shape2);
    } else {
        ui->sliderCenter->show();
        ui->sliderTension->show();
        ui->labelCenter->show();
        ui->labelTension->show();
        ui->transitionGeneric->hide();
    }
}

double TransitionManagerWidget::getShapeValue(int shape, double range) {
    return ((range*2.0f) * shape / 100000.0f) - range;
}

int TransitionManagerWidget::getShapeValue(double shape, double range) {
    return (int)(((shape+range)/(range*2.0f)) * 100000.0f);
}

void TransitionManagerWidget::Update() {
    emit valuesChanged();
}

TransitionManagerWidget::~TransitionManagerWidget()
{
    delete ui;
}

void TransitionManagerWidget::on_sliderCenter_sliderMoved(int position)
{
    transition->setCenter((double)position);
    Update();
}

void TransitionManagerWidget::on_sliderTension_sliderMoved(int position)
{
    transition->setTension((double)position);
    Update();
}

void TransitionManagerWidget::on_comboMode_currentIndexChanged(int index)
{
    transition->setMode(index);
    Update();
}

void TransitionManagerWidget::on_comboType_currentIndexChanged(int index)
{
    if(index == 0) transition->setTransitionType(TransitionManager::BasicTransition);
    if(index == 1) transition->setTransitionType(TransitionManager::BumpTransition);
    if(index == 2) transition->setTransitionType(TransitionManager::LinearTransition);
    if(index == 4) transition->setTransitionType(TransitionManager::QuinticTransition);
    if(index == 5) transition->setTransitionType(TransitionManager::GenericTransition);

    buildGui();
    Update();
}

void TransitionManagerWidget::on_useCustomSlope_clicked(bool checked)
{
    tGeneric->useSlope = checked;
    tGeneric->slope = (double)ui->inputSlope->value();
    Update();
}

void TransitionManagerWidget::on_inputSlope_valueChanged(double arg1)
{
    tGeneric->slope = (double)arg1;

    ui->sliderSlope->blockSignals(true);
    ui->sliderSlope->setValue(getShapeValue(tGeneric->slope, rangeSlope));
    ui->sliderSlope->blockSignals(false);

    Update();
}


void TransitionManagerWidget::on_sliderSlope_sliderMoved(int position)
{
    tGeneric->slope = getShapeValue(position, rangeSlope);

    ui->inputSlope->blockSignals(true);
    ui->inputSlope->setValue(tGeneric->slope);
    ui->inputSlope->blockSignals(false);

    Update();
}

void TransitionManagerWidget::on_inputShape1_valueChanged(double arg1)
{
    tGeneric->shape1 = (double)arg1;

    ui->sliderShape1->blockSignals(true);
    ui->sliderShape1->setValue(getShapeValue(tGeneric->shape1, rangeShape1));
    ui->sliderShape1->blockSignals(false);

    Update();
}


void TransitionManagerWidget::on_sliderShape1_sliderMoved(int position)
{
    tGeneric->shape1 = getShapeValue(position, rangeShape1);

    ui->inputShape1->blockSignals(true);
    ui->inputShape1->setValue(tGeneric->shape1);
    ui->inputShape1->blockSignals(false);

    Update();
}

void TransitionManagerWidget::on_inputShape2_valueChanged(double arg1)
{
    tGeneric->shape2 = (double)arg1;

    ui->sliderShape2->blockSignals(true);
    ui->sliderShape2->setValue(getShapeValue(tGeneric->shape2, rangeShape2));
    ui->sliderShape2->blockSignals(false);

    Update();
}


void TransitionManagerWidget::on_sliderShape2_sliderMoved(int position)
{
    tGeneric->shape2 = getShapeValue(position, rangeShape2);

    ui->inputShape2->blockSignals(true);
    ui->inputShape2->setValue(tGeneric->shape2);
    ui->inputShape2->blockSignals(false);

    Update();
}
