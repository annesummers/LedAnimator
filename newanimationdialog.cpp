#include "newanimationdialog.h"
#include "ui_newanimation.h"

#include "animation.h"

NewAnimationDialog::NewAnimationDialog(QWidget *parent, Engine &engine) :
    QDialog(parent),
    iEngine(engine),
    ui(new Ui::NewAnimation) {
    ui->setupUi(this);
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

/*void NewAnimationDialog::accept() {
    iEngine.setupNewAnimation(iNumRows, iNumColumns, iNumFrames, iFrameFrequency);

    QDialog::accept();
}*/
