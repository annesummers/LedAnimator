/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>
#include <QUndoStack>

#include "selectablegroupgroupwidget.h"
//#include "Animation.h"

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
    void setupNewAnimation(int numRows, int numColumns);

    void setSelectedGroupGroup(SelectableGroupGroupWidget *groupGroup);
    inline bool isSelectedGroupGroup(SelectableGroupGroupWidget* groupGroup) { return groupGroup == iSelectedGroupGroup; }

signals:
    void doQuit();

private slots:
    void saveAnimation();
    void saveAnimationAs();

    void loadAnimation();
    void newAnimation(bool askSaveOld = true);

    void exportAnimation();

    void addTimeAxis();
    void addValueAxis();

    void importBitmap();

    void quit();

private:
    void doSave(QString fileName);
    bool doLoad(QString fileName);
    void doExport(QString fileName);

    void setupUI();

    MainWindow* iMainWindow;
    Animation* iAnimation;

    NewAnimationDialog* iNewAnimationDialog;

    SelectableGroupGroupWidget* iSelectedGroupGroup;
};

#endif // ENGINE_H
