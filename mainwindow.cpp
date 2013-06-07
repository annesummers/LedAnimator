/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#include "mainwindow.h"

#include "led.h"
#include "ledwidget.h"
#include "ledgridgroupwidget.h"
#include "ledgridwidget.h"
#include "playinfowidget.h"
#include "engine.h"
#include "animation.h"
#include "animationdetailswidget.h"
#include "grouplistwidget.h"
#include "leddetails.h"
#include "framedetails.h"

#include "constants.h"

using namespace AnimatorUi;

MainWindow::MainWindow(Engine& engine) :
    iEngine(engine) {

    setWindowTitle(APP_NAME);

    readSettings();

    iUndoStack = new QUndoStack(this);

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setObjectName(QString::fromUtf8("centralWidget"));

    QGridLayout* gridLayout = new QGridLayout(centralWidget);
    gridLayout->setSpacing(6);
    gridLayout->setContentsMargins(11, 11, 11, 11);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));

    LedGridGroupWidget* ledGridGroupWidget = new LedGridGroupWidget(centralWidget, engine);
    LedGridWidget* ledGridWidget = new LedGridWidget(centralWidget, engine.animation(), *ledGridGroupWidget);
    ledGridWidget->setObjectName(QString::fromUtf8("LedGridWidget"));

    connect(&engine.animation(), SIGNAL(newLed(int, int)), ledGridWidget, SLOT(addLed(int, int)));
    connect(&engine.animation(), SIGNAL(newSocket(int, int)), ledGridWidget, SLOT(addSocket(int, int)));
    connect(&engine.animation(), SIGNAL(ledDeleted(int, int, int)), ledGridWidget, SLOT(ledDeleted(int, int, int)));
    connect(&engine.animation(), SIGNAL(ledMoved(int, int, int, int)), ledGridWidget, SLOT(ledMoved(int, int, int, int)));

    gridLayout->addWidget(ledGridWidget, 0, 0, 2, 1);

    LedDetailsWidget* ledDetailsWidget = new LedDetailsWidget(centralWidget);
    ledDetailsWidget->setObjectName(QString::fromUtf8("ledDetailsWidget"));

    connect(ledGridWidget, SIGNAL(currentLedDetails(int, int, int, QColor)), ledDetailsWidget, SLOT(currentLedDetails(int, int, int, QColor)));

    gridLayout->addWidget(ledDetailsWidget, 0, 1, 2, 1);

    FrameDetailsWidget* frameDetailsWidget = new FrameDetailsWidget(centralWidget);
    frameDetailsWidget->setObjectName(QString::fromUtf8("frameDetailsWidget"));

    connect(&engine.animation(), SIGNAL(currentFrameChanged(int)), frameDetailsWidget, SLOT(currentFrameChanged(int)));
    connect(&engine.animation(), SIGNAL(numFramesChanged(int)), frameDetailsWidget, SLOT(numFramesChanged(int)));

    gridLayout->addWidget(frameDetailsWidget, 0, 2, 1, 1);

    PlayInfoWidget* playInfoWidget = new PlayInfoWidget(centralWidget, engine.animation());
    playInfoWidget->setObjectName(QString::fromUtf8("PlayInfoWidget"));

    gridLayout->addWidget(playInfoWidget, 1, 2, 1, 1);

    QWidget* widget1 = new QWidget(centralWidget);
    gridLayout->addWidget(widget1, 0, 3, 2, 1);

    AnimationDetailsWidget* animationDetailsWidget = new AnimationDetailsWidget(centralWidget, engine.animation(), engine);
    animationDetailsWidget->setObjectName(QString::fromUtf8("AnimationDetailsWidget"));

    connect(&engine.animation(), SIGNAL(ledDeleted(int, int, int)), animationDetailsWidget, SLOT(ledDeleted(int, int, int)));
    connect(&engine.animation(), SIGNAL(ledRenumbered(int,int,int)), animationDetailsWidget, SLOT(ledRenumbered(int, int, int)));
    connect(&engine.animation(), SIGNAL(numFramesChanged(int)), animationDetailsWidget, SLOT(numFramesChanged(int)));
    connect(&engine.animation(), SIGNAL(currentFrameChanged(int)), animationDetailsWidget, SLOT(currentFrameChanged(int)));

    gridLayout->addWidget(animationDetailsWidget, 2, 0, 1, 4);

    setCentralWidget(centralWidget);

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
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

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
    connect(iCutAction, SIGNAL(triggered()), animationDetailsWidget, SLOT(cutSelected()));
    connect(iCopyAction, SIGNAL(triggered()), ledGridGroupWidget, SLOT(copySelected()));
    connect(iCopyAction, SIGNAL(triggered()), animationDetailsWidget, SLOT(copySelected()));
    connect(iPasteAction, SIGNAL(triggered()), ledGridGroupWidget, SLOT(paste()));
    connect(iPasteAction, SIGNAL(triggered()), animationDetailsWidget, SLOT(paste()));
    connect(iPasteWrapAction, SIGNAL(triggered()), ledGridGroupWidget, SLOT(pasteWrap()));
    connect(iPasteWrapAction, SIGNAL(triggered()), animationDetailsWidget, SLOT(pasteWrap()));

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
    QAction* setNumFramesAction = animationMenu->addAction("Set &number of frames");
    QAction* setFrameFrequencyAction = animationMenu->addAction("Set &frame frequency");

    connect(copyToClipboardAction, SIGNAL(triggered()), &(iEngine.animation()), SLOT(copyToClipboard()));
    connect(setNumFramesAction, SIGNAL(triggered()), &iEngine, SLOT(setNumFrames()));
    connect(setFrameFrequencyAction, SIGNAL(triggered()), &iEngine, SLOT(setFrameFrequency()));

    menuBar()->addMenu(animationMenu);
}

MainWindow::~MainWindow() {
    writeSettings();
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
    } else {
        event->ignore();
    }
}

