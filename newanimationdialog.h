/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#ifndef NEWANIMATION_H
#define NEWANIMATION_H

#include <QDialog>

class Engine;

namespace Ui {
class NewAnimation;
}

namespace AnimatorUi {

class NewAnimationDialog : public QDialog {
    Q_OBJECT
    
public:
    explicit NewAnimationDialog(QWidget *parent, Engine &engine);
    ~NewAnimationDialog();

    int iNumColumns;
    int iNumRows;

private slots:
    void on_iNumRowsSpin_valueChanged(int newValue);
    void on_iNumColumnsSpin_valueChanged(int newValue);
    
private:
    Ui::NewAnimation *ui;

    Engine& iEngine;
};
}
#endif // NEWANIMATION_H
