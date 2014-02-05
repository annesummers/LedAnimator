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
#include <QSignalMapper>

class Engine;

namespace AnimatorUi {

class SelectableGroupGroupWidget;
class ValueAxisDetailsWidget;

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

public slots:
    void showTimeAxisDetails();
    void showValueAxisDetails(int);

    void deleteValueAxisDetails(int axisNumber);

protected:
    void closeEvent(QCloseEvent *event);
    
private:
    Engine&         iEngine;

    QAction*        iCutAction;
    QAction*        iCopyAction;
    QAction*        iPasteAction;
    QAction*        iPasteWrapAction;

    QAction*        iAddTimeAxisAction;
    QAction*        iAddValueAxisAction;

    QMenu*        iDeleteValueAxisMenu;

    QWidget*        iTimeAxisMainWidget;
    QMap<int, ValueAxisDetailsWidget*>  iValueAxisDetailsWidgets;

    QUndoStack*     iUndoStack;

    QSignalMapper* iDeleteValueAxisSignalMapper;
};
}
#endif // MAINWINDOW_H
