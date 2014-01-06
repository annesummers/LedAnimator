#include "NewTimeAxisDialog.h"
#include "ui_NewTimeAxisDialog.h"

#include "Engine.h"

using namespace AnimatorUi;

NewTimeAxisDialog::NewTimeAxisDialog(QWidget *parent, Engine& engine) :
    QDialog(parent),
    iPriority(kPriorityLow),
    iOpaque(false),
    ui(new Ui::NewTimeAxisDialog),
    iEngine(engine) {
    ui->setupUi(this);
}

NewTimeAxisDialog::~NewTimeAxisDialog() {
    delete ui;
}

void NewTimeAxisDialog::on_iNumFramesSpin_valueChanged(int newValue) {
    iNumFrames = newValue;
}

void NewTimeAxisDialog::on_iSpeedSpin_valueChanged(int newValue) {
    iSpeed = newValue;
}
