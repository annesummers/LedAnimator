/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>

#include "mainwindow.h"

class Animation;
class Led;

class Engine : public QObject {
    Q_OBJECT

public:
    explicit Engine(QObject *parent = 0);

    void start();

    Animation& animation() const;

    bool askSaveAnimation();
    
signals:
    
private slots:
    void saveAnimation();
    void saveAnimationAs();

    void loadAnimation();
    void newAnimation();

private:
    void doSave(QString fileName);
    void doLoad(QString fileName);

    void setupUI();

    MainWindow* iMainWindow;
    Animation* iAnimation;
};

#endif // ENGINE_H
