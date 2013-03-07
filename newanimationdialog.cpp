/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#include "newanimationdialog.h"
#include "ui_newanimation.h"

#include "animation.h"

NewAnimationDialog::NewAnimationDialog(QWidget *parent, Engine &engine) :
    QDialog(parent),
    ui(new Ui::NewAnimation),
    iEngine(engine) {
    ui->setupUi(this);

    iNumFrames = ui->iNumFramesSpin->value();
    iFrameFrequency = ui->iFrameFrequencySpin->value();
    iNumRows = ui->iNumRowsSpin->value();
    iNumColumns = ui->iNumColumnsSpin->value();
}

NewAnimationDialog::~NewAnimationDialog() {
    delete ui;
}

void NewAnimationDialog::on_iNumFramesSpin_valueChanged(int newValue) {
    iNumFrames = newValue;
}

void NewAnimationDialog::on_iFrameFrequencySpin_valueChanged(int newValue) {
    iFrameFrequency = newValue;
}

void NewAnimationDialog::on_iNumRowsSpin_valueChanged(int newValue) {
    iNumRows = newValue;
}

void NewAnimationDialog::on_iNumColumnsSpin_valueChanged(int newValue) {
    iNumColumns = newValue;
}
