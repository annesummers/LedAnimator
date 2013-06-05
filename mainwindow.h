/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QUndoStack>
#include <QUndoView>

class Engine;

namespace AnimatorUi {

class SelectableGroupGroupWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT    

public:
    explicit MainWindow(Engine& engine);
    virtual ~MainWindow();

    void readSettings();
    void writeSettings();

    void setSelectedGroupGroup(SelectableGroupGroupWidget *groupGroup);
    inline bool isSelectedGroupGroup(SelectableGroupGroupWidget* groupGroup) { return groupGroup == iSelectedGroupGroup; }

    void setEnabledPasteActions(bool enabled);

    inline QUndoStack& undoStack() { return *iUndoStack; }

protected:
    void closeEvent(QCloseEvent *event);
    
private:
    Engine&         iEngine;

    QAction*        iCutAction;
    QAction*        iCopyAction;
    QAction*        iPasteAction;
    QAction*        iPasteWrapAction;

    QUndoStack*     iUndoStack;

    SelectableGroupGroupWidget* iSelectedGroupGroup;
};
}
#endif // MAINWINDOW_H
