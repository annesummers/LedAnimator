#include "NewValueAxisDialog.h"
#include "ui_NewValueAxisDialog.h"

#include "Engine.h"

using namespace AnimatorUi;

NewValueAxisDialog::NewValueAxisDialog(QWidget *parent, Engine &engine) :
    QDialog(parent),
    iZeroValue(0),
    iPriority(kPriorityLow),
    iOpaque(false),
    ui(new Ui::NewValueAxisDialog),
    iEngine(engine) {
    ui->setupUi(this);

    iLowValue = ui->iLowValueSpin->value();
    iHighValue = ui->iHighValueSpin->value();
}

NewValueAxisDialog::~NewValueAxisDialog() {
    delete ui;
}

void NewValueAxisDialog::on_iLowValueSpin_valueChanged(int newValue) {
    iLowValue = newValue;
}

void NewValueAxisDialog::on_iHighValueSpin_valueChanged(int newValue) {
    iHighValue = newValue;
}

