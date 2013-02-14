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
class NewAnimationDialog;

class Engine : public QObject {
    Q_OBJECT

public:
    explicit Engine(QObject *parent = 0);

    void start();

    inline Animation& animation() const { return *iAnimation; }

    bool askSaveAnimation();  
    void setupNewAnimation(int numRows, int numColumns, int numFrames, int frameFrequency);

public slots:
    void setNumFrames();
    void setFrameFrequency();
    
private slots:
    void saveAnimation();
    void saveAnimationAs();

    void loadAnimation();
    void newAnimation();

private:
    void doSave(QString fileName);
    bool doLoad(QString fileName);

    void setupUI();

    MainWindow* iMainWindow;
    Animation* iAnimation;

    NewAnimationDialog* iNewAnimationDialog;
};

#endif // ENGINE_H
