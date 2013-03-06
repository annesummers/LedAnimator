/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>

//#include "mainwindow.h"

namespace AnimatorModel {
class Animation;
}

namespace AnimatorUi {
class NewAnimationDialog;
class MainWindow;
}

using namespace AnimatorUi;
using namespace AnimatorModel;

class Engine : public QObject {
    Q_OBJECT

public:
    explicit Engine(QObject *parent = 0);

    void start();

    inline AnimatorModel::Animation& animation() const { return *iAnimation; }

    bool askSaveAnimation();  
    void setupNewAnimation(int numRows, int numColumns, int numFrames, int frameFrequency);

public slots:
    
private slots:
    void saveAnimation();
    void saveAnimationAs();

    void loadAnimation();
    void newAnimation(bool askSaveOld = true);

    void exportAnimation();

    void setNumFrames();
    void setFrameFrequency();

private:
    void doSave(QString fileName, bool withPositions = true);
    bool doLoad(QString fileName);

    void setupUI();

    MainWindow* iMainWindow;
    Animation* iAnimation;

    NewAnimationDialog* iNewAnimationDialog;
};

#endif // ENGINE_H
