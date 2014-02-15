#include "renumberleddialog.h"
#include "ui_renumberleddialog.h"

#include "Animation.h"

using namespace AnimatorUi;
using namespace AnimatorModel;

RenumberLedDialog::RenumberLedDialog(QWidget *parent, Animation &animation) :
    QDialog(parent),
    ui(new Ui::RenumberLedDialog),
    iAnimation(animation),
    iNewNumber(INVALID) {
    ui->setupUi(this);

    if(animation.ledsMissing()) {
        int index = 0;
        int number;
        foreach(number, animation.missingLedNumbers()) {
            ui->ledNumberCombo->insertItem(index++, QString("%1").arg(number));
        }
    }

    //ui->ledNumberCombo->insertItem(0, QString("%1").arg(animation.nextLedNumber()));
    ui->ledNumberCombo->setCurrentIndex(0);

    setWindowTitle(QString("Renumber Led"));
}

RenumberLedDialog::~RenumberLedDialog() {
    delete ui;
}

void RenumberLedDialog::on_ledNumberCombo_currentIndexChanged(const QString &chosenNumber) {
    if(ui->buttonGroup->checkedButton() == ui->missing) {
        iNewNumber = chosenNumber.toInt();
    }
}

void RenumberLedDialog::on_ledNumberSpin_valueChanged(int newValue) {
    if(ui->buttonGroup->checkedButton() == ui->choose) {
        iNewNumber = newValue;
    }
}

void RenumberLedDialog::on_missing_toggled() {
    if(ui->missing->isChecked()) {
        iNewNumber = ui->ledNumberCombo->currentIndex();
    }
}

void RenumberLedDialog::on_choose_toggled() {
    if(ui->choose->isChecked()) {
        iNewNumber = ui->ledNumberSpin->value();
    }

}
