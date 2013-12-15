/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#include "mainwindow.h"

#include "Led.h"
#include "ledwidget.h"
#include "ledgridgroupwidget.h"
#include "ledgridwidget.h"
#include "TimeAxisPlayWidget.h"
#include "engine.h"
#include "Animation.h"
#include "TimeAxisDetailsWidget.h"
#include "ValueAxisDetailsWidget.h"
#include "grouplistwidget.h"
#include "LedDetailsWidget.h"
#include "FrameDetailsWidget.h"

#include "constants.h"

using namespace AnimatorUi;

MainWindow::MainWindow(Engine& engine) :
    iEngine(engine) {

    setWindowTitle(APP_NAME);

    readSettings();

    iUndoStack = new QUndoStack(this);

    LedGridGroupWidget* ledGridGroupWidget = new LedGridGroupWidget(this, engine);
    LedGridWidget* ledGridWidget = new LedGridWidget(this, engine.animation(), *ledGridGroupWidget);
    ledGridWidget->setObjectName(QString::fromUtf8("LedGridWidget"));

    connect(&engine.animation(), SIGNAL(newLed(int, int)), ledGridWidget, SLOT(addLed(int, int)));
    connect(&engine.animation(), SIGNAL(newSocket(int, int)), ledGridWidget, SLOT(addSocket(int, int)));
    connect(&engine.animation(), SIGNAL(ledDeleted(int, int, int)), ledGridWidget, SLOT(ledDeleted(int, int, int)));
    connect(&engine.animation(), SIGNAL(ledMoved(int, int, int, int)), ledGridWidget, SLOT(ledMoved(int, int, int, int)));

    LedDetailsWidget* ledDetailsWidget = new LedDetailsWidget(NULL);
    ledDetailsWidget->setObjectName(QString::fromUtf8("ledDetailsWidget"));
    ledDetailsWidget->show();
    connect(ledGridWidget, SIGNAL(currentLedDetails(int, int, int, QColor)), ledDetailsWidget, SLOT(currentLedDetails(int, int, int, QColor)));

    setCentralWidget(ledGridGroupWidget);

    connect(&engine.animation(), SIGNAL(timeAxisAdded()), this, SLOT(showTimeAxisDetails()));
    connect(&engine.animation(), SIGNAL(valueAxisAdded(int)), this, SLOT(showValueAxisDetails(int)));

    QMenu* fileMenu = new QMenu("&File", this);
    QAction* newAction = fileMenu->addAction("&New animation...");
    newAction->setShortcuts(QKeySequence::New);
    QAction* openAction = fileMenu->addAction("&Open animation...");
    openAction->setShortcuts(QKeySequence::Open);
    QAction* saveAction = fileMenu->addAction("&Save");
    saveAction->setShortcuts(QKeySequence::Save);
    QAction* saveAsAction = fileMenu->addAction("Save &As...");
    saveAsAction->setShortcuts(QKeySequence::SaveAs);
    fileMenu->addSeparator();
    QAction* importAction = fileMenu->addAction("&Import...");
    QAction* exportAction = fileMenu->addAction("&Export...");
    fileMenu->addSeparator();
    QAction* quitAction = fileMenu->addAction("E&xit");
    quitAction->setShortcuts(QKeySequence::Quit);

    connect(newAction, SIGNAL(triggered()), &iEngine, SLOT(newAnimation()));
    connect(openAction, SIGNAL(triggered()), &iEngine, SLOT(loadAnimation()));
    connect(saveAction, SIGNAL(triggered()), &iEngine, SLOT(saveAnimation()));
    connect(saveAsAction, SIGNAL(triggered()), &iEngine, SLOT(saveAnimationAs()));
    connect(importAction, SIGNAL(triggered()), &iEngine, SLOT(importBitmap()));
    connect(exportAction, SIGNAL(triggered()), &iEngine, SLOT(exportAnimation()));
    connect(quitAction, SIGNAL(triggered()), &iEngine, SLOT(quit()));

    menuBar()->addMenu(fileMenu);

    QMenu* editMenu = new QMenu("&Edit", this);
    iCutAction = editMenu->addAction("&Cut");
    iCutAction->setShortcuts(QKeySequence::Cut);
    iCopyAction = editMenu->addAction("C&opy");
    iCopyAction->setShortcuts(QKeySequence::Copy);
    iPasteAction = editMenu->addAction("&Paste");
    iPasteAction->setShortcuts(QKeySequence::Paste);
    iPasteWrapAction = editMenu->addAction("Paste &wrap");

    connect(iCutAction, SIGNAL(triggered()), ledGridGroupWidget, SLOT(cutSelected()));
    //connect(iCutAction, SIGNAL(triggered()), animationDetailsWidget, SLOT(cutSelected()));
    connect(iCopyAction, SIGNAL(triggered()), ledGridGroupWidget, SLOT(copySelected()));
   // connect(iCopyAction, SIGNAL(triggered()), animationDetailsWidget, SLOT(copySelected()));
    connect(iPasteAction, SIGNAL(triggered()), ledGridGroupWidget, SLOT(paste()));
  //  connect(iPasteAction, SIGNAL(triggered()), animationDetailsWidget, SLOT(paste()));
    connect(iPasteWrapAction, SIGNAL(triggered()), ledGridGroupWidget, SLOT(pasteWrap()));
 //   connect(iPasteWrapAction, SIGNAL(triggered()), animationDetailsWidget, SLOT(pasteWrap()));

    iCutAction->setEnabled(false);
    iCopyAction->setEnabled(false);
    setEnabledPasteActions(false);

    editMenu->addSeparator();

    QAction* undoAction = iUndoStack->createUndoAction(this, tr("&Undo"));
    undoAction->setShortcuts(QKeySequence::Undo);

    QAction* redoAction = iUndoStack->createRedoAction(this, tr("&Redo"));
    redoAction->setShortcuts(QKeySequence::Redo);

    editMenu->addAction(undoAction);
    editMenu->addAction(redoAction);

    menuBar()->addMenu(editMenu);

    QMenu* animationMenu = new QMenu("&Animation", this);
    QAction* copyToClipboardAction = animationMenu->addAction("&Copy to clipboard");
    animationMenu->addSeparator();
    iAddTimeAxisAction = animationMenu->addAction("Add &time axis");
    iAddValueAxisAction = animationMenu->addAction("Add &value axis");
    //QAction* addFramesAction = animationMenu->addAction("&Add frames");
    //QAction* setNumFramesAction = animationMenu->addAction("Set &number of frames");
    //animationMenu->addSeparator();
    //QAction* setFrameFrequencyAction = animationMenu->addAction("Set &frame frequency");

    connect(copyToClipboardAction, SIGNAL(triggered()), &(iEngine.animation()), SLOT(copyToClipboard()));
    connect(iAddTimeAxisAction, SIGNAL(triggered()), &iEngine, SLOT(addTimeAxis()));
    connect(iAddValueAxisAction, SIGNAL(triggered()), &iEngine, SLOT(addValueAxis()));
    iAddTimeAxisAction->setEnabled(false);
    iAddValueAxisAction->setEnabled(true);
    //connect(addFramesAction, SIGNAL(triggered()), &iEngine, SLOT(addFrames()));
    //connect(setNumFramesAction, SIGNAL(triggered()), &iEngine, SLOT(setNumFrames()));
    //connect(setFrameFrequencyAction, SIGNAL(triggered()), &iEngine, SLOT(setFrameFrequency()));

    menuBar()->addMenu(animationMenu);
}

MainWindow::~MainWindow() {
    writeSettings();
}

void MainWindow::showTimeAxisDetails() {
    // TODO store so we can delete
    QWidget* widget1 = new QWidget(NULL);

    FrameDetailsWidget* frameDetailsWidget = new FrameDetailsWidget(widget1);
    frameDetailsWidget->setObjectName(QString::fromUtf8("frameDetailsWidget"));

    connect(iEngine.animation().timeAxis(), SIGNAL(currentFrameChanged(int)), frameDetailsWidget, SLOT(currentFrameChanged(int)));
    connect(iEngine.animation().timeAxis(), SIGNAL(highValueChanged(int)), frameDetailsWidget, SLOT(highValueChanged(int)));
    //connect(&engine.animation(), SIGNAL(framesInserted(int,int)), frameDetailsWidget, SLOT(framesInserted(int, int)));

    frameDetailsWidget->highValueChanged(iEngine.animation().timeAxis()->highValue());

    TimeAxisPlayWidget* playInfoWidget = new TimeAxisPlayWidget(widget1, iEngine.animation(), *iEngine.animation().timeAxis());
    playInfoWidget->setObjectName(QString::fromUtf8("PlayInfoWidget"));

    TimeAxisDetailsWidget* axisDetailsWidget = new TimeAxisDetailsWidget(widget1, iEngine.animation(), *iEngine.animation().timeAxis(), iEngine);
    axisDetailsWidget->setObjectName(QString::fromUtf8("AnimationDetailsWidget"));
   // animationDetailsWidget->show();

    QGridLayout* gridLayout = new QGridLayout();
    gridLayout->setObjectName(QString::fromUtf8("verticalLayout"));

    gridLayout->addWidget(playInfoWidget, 0, 0, 1, 1);
    gridLayout->addWidget(frameDetailsWidget, 0, 1, 1, 1);
    gridLayout->addWidget(axisDetailsWidget, 1, 0, 1, 2);

    QHBoxLayout* mainLayout = new QHBoxLayout(widget1);
    mainLayout->setObjectName(QString::fromUtf8("mainLayout"));

    mainLayout->addLayout(gridLayout);

    connect(&iEngine.animation(), SIGNAL(ledDeleted(int, int, int)), axisDetailsWidget, SLOT(ledDeleted(int, int, int)));
    connect(&iEngine.animation(), SIGNAL(ledRenumbered(int,int,int)), axisDetailsWidget, SLOT(ledRenumbered(int, int, int)));
    connect(iEngine.animation().timeAxis(), SIGNAL(currentFrameChanged(int)), axisDetailsWidget, SLOT(currentFrameChanged(int)));
   // connect(&engine.animation(), SIGNAL(numFramesChanged(int)), animationDetailsWidget, SLOT(numFramesChanged(int)));
   // connect(&engine.animation(), SIGNAL(framesInserted(int,int)), animationDetailsWidget, SLOT(framesInserted(int, int)));

    widget1->show();
}

void MainWindow::showValueAxisDetails(int axisNumber) {
    // TODO store so we can delete
    ValueAxisDetailsWidget* axisDetailsWidget = new ValueAxisDetailsWidget(NULL, iEngine.animation(), iEngine.animation().axisAt(axisNumber), iEngine);
    axisDetailsWidget->setObjectName(QString::fromUtf8("AnimationDetailsWidget"));
    axisDetailsWidget->show();

    connect(&iEngine.animation(), SIGNAL(ledDeleted(int, int, int)), axisDetailsWidget, SLOT(ledDeleted(int, int, int)));
    connect(&iEngine.animation(), SIGNAL(ledRenumbered(int,int,int)), axisDetailsWidget, SLOT(ledRenumbered(int, int, int)));
}

void MainWindow::writeSettings() {
    QSettings settings;

    settings.beginGroup(SETTINGS_MAIN_WINDOW);
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.endGroup();
}

void MainWindow::readSettings() {
    QSettings settings;

    settings.beginGroup(SETTINGS_MAIN_WINDOW);
    resize(settings.value("size", QSize(600, 300)).toSize());
    move(settings.value("pos", QPoint(200, 200)).toPoint());
    settings.endGroup();
}

void MainWindow::setEnabledPasteActions(bool enabled) {
    iPasteAction->setEnabled(enabled);
    iPasteWrapAction->setEnabled(enabled);
}

void MainWindow::setEnabledCutAction(bool enabled) {
    iCutAction->setEnabled(enabled);
}

void MainWindow::setEnabledCopyAction(bool enabled) {
    iCopyAction->setEnabled(enabled);
}

// events --------------------------------------

void MainWindow::closeEvent(QCloseEvent *event) {
    if (iEngine.askSaveAnimation()) {
        event->accept();
        delete this;
    } else {
        event->ignore();
    }
}
