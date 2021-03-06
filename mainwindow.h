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
#include <QSize>

class Engine;

namespace AnimatorUi {

class SelectableGroupGroupWidget;
class ValueAxisDetailsWidget;
class LedDetailsWidget;

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

private slots:
    void addTimeAxisDetails();
    void addValueAxisDetails(int);

    void showValueAxisDetails(int axisNumber);
    void showTimeAxisDetails();

    void hideValueAxisDetails(int axisNumber);
    void hideTimeAxisDetails();

    void deleteValueAxisDetails(int axisNumber);

    void handleMaxSizeChanged(QSize maximumSize);

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

    QAction*        iShowTimeAxisAction;

    QMenu*          iAnimationMenu;

    QMenu*          iDeleteValueAxisMenu;
    QMenu*          iShowValueAxisMenu;

    QWidget*        iTimeAxisMainWidget;
    QList<ValueAxisDetailsWidget*>  iValueAxisDetailsWidgets;

    LedDetailsWidget* iLedDetailsWidget;

    QUndoStack*     iUndoStack;

    QSignalMapper* iDeleteValueAxisSignalMapper;
    QSignalMapper* iShowValueAxisSignalMapper;

    QSize iPreferredSize;
};
}
#endif // MAINWINDOW_H
