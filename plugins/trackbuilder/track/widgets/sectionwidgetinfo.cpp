#include <QSpinBox>
#include <QLayout>

#include "sectionwidgetinfo.h"
#include "ui_sectionwidgetinfo.h"
#include "track/section.h"
#include "track/mnode.h"
#include "track/track.h"

#include "sectionwidget.h"

#include <QDebug>

SectionWidgetInfo::SectionWidgetInfo(section *_Section, SectionWidget *_parentSectionWidget, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SectionWidgetInfo)
{
    ui->setupUi(this);

    Section = _Section;
    parentSectionWidget = _parentSectionWidget;

    ui->inputPosX->setValue(Section->parent->startPos.x);
    ui->inputPosY->setValue(Section->parent->startPos.y);
    ui->inputPosZ->setValue(Section->parent->startPos.z);

    ui->inputPitch->setValue(Section->parent->startYaw);
    ui->inputYaw->setValue(Section->parent->anchorNode->getDirection());
    ui->inputSpeed->setValue(Section->parent->anchorNode->fVel);
    ui->inputHeartline->setValue(Section->parent->fHeart);
}

void SectionWidgetInfo::UpdateValues() {
    if(!Section->lNodes.size()) return;

    mnode *outputValues = Section->lNodes.at(Section->lNodes.size()-1);
    mnode *inputValues = Section->lNodes.at(0);

    float gravity = 9.81;

    // Calculate Panel Values
    ui->labelOrigin->setText(QString().sprintf("x: %2.2fm | y: %2.2fm | z: %2.2fm", outputValues->vPos.x, outputValues->vPos.y, outputValues->vPos.z));

    ui->labelBanking->setText(QString().sprintf("%2.2f%c / %2.2f%c", inputValues->fRoll, 0xB0 , outputValues->fRoll, 0xB0));
    ui->labelBanking->setToolTip(QString().sprintf("%2.2f radian / %2.2f radian", inputValues->fRoll / (180/M_PI), outputValues->fRoll / (180/M_PI)));

    ui->labelSpeed->setText(QString().sprintf("%2.2f ms / %2.2f ms", inputValues->fVel, outputValues->fVel));
    ui->labelSpeed->setToolTip(QString().sprintf("%2.2f kmh / %2.2f kmh", inputValues->fVel*3.6, outputValues->fVel*3.6));

    ui->labelVer->setText(QString().sprintf("%2.2f g / %2.2f g", inputValues->forceNormal, outputValues->forceNormal));
    ui->labelVer->setToolTip(QString().sprintf("%2.2f Newton / %2.2f Newton", inputValues->forceNormal*gravity, outputValues->forceNormal*gravity));

    ui->labelLat->setText(QString().sprintf("%2.2f g / %2.2f g", inputValues->forceLateral, outputValues->forceLateral));
    ui->labelLat->setToolTip(QString().sprintf("%2.2f Newton / %2.2f Newton", inputValues->forceLateral*gravity, outputValues->forceLateral*gravity));

    ui->labelPitch->setText(QString().sprintf("%2.2f%c / %2.2f%c", inputValues->getPitch(), 0xB0, outputValues->getPitch(), 0xB0));
    ui->labelPitch->setToolTip(QString().sprintf("%2.2f radian / %2.2f radian", inputValues->getPitch() / (180/M_PI), outputValues->getPitch() / (180/M_PI) ));

    ui->labelYaw->setText(QString().sprintf("%2.2f%c / %2.2f%c", inputValues->getDirection(), 0xB0, outputValues->getDirection(), 0xB0));
    ui->labelYaw->setToolTip(QString().sprintf("%2.2f radian / %2.2f radian", inputValues->getDirection()/ (180/M_PI), outputValues->getDirection()/ (180/M_PI)));
}

void SectionWidgetInfo::on_inputPosX_valueChanged(double arg1)
{
    float a = Section->parent->startYaw * M_PI/180.f;
    float b = Section->parent->startPitch * M_PI/180.f;
    float y = Section->parent->anchorNode->fRoll * M_PI/180.f;
    Section->parent->startPos.x = -(cos(a)*sin(b)*cos(y)+sin(a)*sin(y))*Section->parent->fHeart + arg1;

    parentSectionWidget->Update();
}

void SectionWidgetInfo::on_inputPosY_valueChanged(double arg1)
{
    float b = Section->parent->startPitch * M_PI/180.f;
    float y = Section->parent->anchorNode->fRoll * M_PI/180.f;
    Section->parent->startPos.y = (cos(b)*cos(y))*Section->parent->fHeart + arg1;

    parentSectionWidget->Update();
}

void SectionWidgetInfo::on_inputPosZ_valueChanged(double arg1)
{
    float a = Section->parent->startYaw * M_PI/180.f;
    float b = Section->parent->startPitch * M_PI/180.f;
    float y = Section->parent->anchorNode->fRoll * M_PI/180.f;
    Section->parent->startPos.z = (sin(a)*sin(b)*cos(y)-cos(a)*sin(y))*Section->parent->fHeart + arg1;

    parentSectionWidget->Update();
}

void SectionWidgetInfo::on_inputPitch_valueChanged(double arg1)
{
    Section->parent->anchorNode->changePitch(arg1-Section->parent->anchorNode->getPitch(), fabs(Section->parent->anchorNode->fRoll) >= 90.f);

    parentSectionWidget->Update();
}

void SectionWidgetInfo::on_inputYaw_valueChanged(double arg1)
{
    Section->parent->startYaw = arg1;
/*
    ui->inputPitch->blockSignals(true);
    ui->inputPitch->setValue(Section->parent->anchorNode->getPitch());
    ui->inputPitch->blockSignals(false);
*/
    parentSectionWidget->Update();
}

void SectionWidgetInfo::on_inputNormal_valueChanged(double arg1)
{
    Section->parent->anchorNode->forceNormal = arg1;
    parentSectionWidget->Update();
}

void SectionWidgetInfo::on_inputLateral_valueChanged(double arg1)
{
    Section->parent->anchorNode->forceLateral = arg1;
    parentSectionWidget->Update();
}

void SectionWidgetInfo::on_inputSpeed_valueChanged(double arg1)
{
    Section->parent->anchorNode->fVel = arg1;
    parentSectionWidget->Update();
}

void SectionWidgetInfo::on_inputHeartline_valueChanged(double arg1)
{
    Section->parent->fHeart = arg1;
    parentSectionWidget->Update();
}

SectionWidgetInfo::~SectionWidgetInfo()
{
    delete ui;
}
