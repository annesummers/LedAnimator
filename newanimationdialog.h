#ifndef NEWANIMATION_H
#define NEWANIMATION_H

#include <QDialog>

class Engine;

namespace Ui {
class NewAnimation;
}

class NewAnimationDialog : public QDialog {
    Q_OBJECT
    
public:
    explicit NewAnimationDialog(QWidget *parent, Engine &engine);
    ~NewAnimationDialog();

    int iNumFrames;
    int iFrameFrequency;

    int iNumColumns;
    int iNumRows;

private slots:
    void on_iNumFramesSpin_valueChanged(int newValue);
    void on_iFrameFrequencySpin_valueChanged(int newValue);
    void on_iNumRowsSpin_valueChanged(int newValue);
    void on_iNumColumnsSpin_valueChanged(int newValue);

   // void accept();
    
private:
    Ui::NewAnimation *ui;

    Engine& iEngine;
};

#endif // NEWANIMATION_H
