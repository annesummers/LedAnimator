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
};

#endif // MAINWINDOW_H
