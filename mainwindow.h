/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>

class Engine;

//namespace Ui {
//class MainWindow;
//}

namespace AnimatorUi {

class MainWindow : public QMainWindow {
    Q_OBJECT    

public:
    explicit MainWindow(Engine& engine);
    virtual ~MainWindow();

    void readSettings();
    void writeSettings();

public slots:

protected:
    void closeEvent(QCloseEvent *event);
    
private:
    Engine&         iEngine;

   // Ui::MainWindow *ui;
};
}
#endif // MAINWINDOW_H
