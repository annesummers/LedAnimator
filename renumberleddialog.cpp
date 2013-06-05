#include "renumberleddialog.h"
#include "ui_renumberleddialog.h"

#include "animation.h"

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

    ui->ledNumberCombo->insertItem(0, QString("%1").arg(animation.nextLedNumber()));
    ui->ledNumberCombo->setCurrentIndex(0);
}

RenumberLedDialog::~RenumberLedDialog() {
    delete ui;
}

void AnimatorUi::RenumberLedDialog::on_ledNumberCombo_currentIndexChanged(const QString &chosenNumber) {
    iNewNumber = chosenNumber.toInt();
}
