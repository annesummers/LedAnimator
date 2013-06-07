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

    inline QUndoStack& undoStack() { return *iUndoStack; }

    void setEnabledPasteActions(bool enabled);
    void setEnabledCutAction(bool enabled);
    void setEnabledCopyAction(bool enabled);

protected:
    void closeEvent(QCloseEvent *event);
    
private:
    Engine&         iEngine;

    QAction*        iCutAction;
    QAction*        iCopyAction;
    QAction*        iPasteAction;
    QAction*        iPasteWrapAction;

    QUndoStack*     iUndoStack;
};
}
#endif // MAINWINDOW_H
