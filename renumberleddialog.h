#ifndef RENUMBERLEDDIALOG_H
#define RENUMBERLEDDIALOG_H

#include <QDialog>

namespace Ui {
class RenumberLedDialog;
}

namespace AnimatorModel {
class Animation;
}

namespace AnimatorUi {

using namespace AnimatorModel;

class RenumberLedDialog : public QDialog {
    Q_OBJECT
    
public:
    explicit RenumberLedDialog(QWidget *parent, Animation& animation);
    ~RenumberLedDialog();

    inline int newNumber() const { return iNewNumber; }
    
private slots:
    void on_ledNumberCombo_currentIndexChanged(const QString &chosenNumber);
    void on_ledNumberSpin_valueChanged(int newValue);
    void on_missing_toggled();
    void on_choose_toggled();

private:
    Ui::RenumberLedDialog *ui;

    Animation& iAnimation;

    int iNewNumber;
};
}

#endif // RENUMBERLEDDIALOG_H
