/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#include "NewAnimationDialog.h"
#include "ui_NewAnimationDialog.h"

#include "Animation.h"

NewAnimationDialog::NewAnimationDialog(QWidget *parent, Engine &engine) :
    QDialog(parent),
    ui(new Ui::NewAnimation),
    iEngine(engine) {
    ui->setupUi(this);

    iNumRows = ui->iNumRowsSpin->value();
    iNumColumns = ui->iNumColumnsSpin->value();

    setWindowTitle(QString("New Animation Details"));
}

NewAnimationDialog::~NewAnimationDialog() {
    delete ui;
}

void NewAnimationDialog::on_iNumRowsSpin_valueChanged(int newValue) {
    iNumRows = newValue;
}

void NewAnimationDialog::on_iNumColumnsSpin_valueChanged(int newValue) {
    iNumColumns = newValue;
}
