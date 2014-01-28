#ifndef NEWTIMEAXISDIALOG_H
#define NEWTIMEAXISDIALOG_H

#include <QDialog>

class Engine;

namespace Ui {
class NewTimeAxisDialog;
}

namespace AnimatorUi {

class NewTimeAxisDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewTimeAxisDialog(QWidget *parent, Engine &engine);
    ~NewTimeAxisDialog();

    int iNumFrames;
    int iFrameRate;

    int iPriority;
    bool iOpaque;

private slots:
    void on_iNumFramesSpin_valueChanged(int newValue);
    void on_iFrameRateSpin_valueChanged(int newValue);

private:
    Ui::NewTimeAxisDialog *ui;

    Engine& iEngine;
};
}

#endif // NEWTIMEAXISDIALOG_H
