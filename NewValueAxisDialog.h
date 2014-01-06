#ifndef NEWVALUEAXISDIALOG_H
#define NEWVALUEAXISDIALOG_H

#include <QDialog>

class Engine;

namespace Ui {
class NewValueAxisDialog;
}

namespace AnimatorUi {

class NewValueAxisDialog : public QDialog {
    Q_OBJECT

public:
    explicit NewValueAxisDialog(QWidget *parent, Engine &engine);
    ~NewValueAxisDialog();

    int iLowValue;
    int iHighValue;
    int iZeroValue;

    int iPriority;
    bool iOpaque;

private slots:
    void on_iLowValueSpin_valueChanged(int newValue);
    void on_iHighValueSpin_valueChanged(int newValue);

private:
    Ui::NewValueAxisDialog *ui;

    Engine& iEngine;
};

}

#endif // NEWVALUEAXISDIALOG_H
