/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#include "mainwindow.h"

#include "led.h"
#include "ledwidget.h"
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

    ColourGroupGroupWidget* gridGroupGroup = new ColourGroupGroupWidget(centralWidget);
    LedGridWidget* ledGridWidget = new LedGridWidget(centralWidget, engine.animation(), *iUndoStack, *gridGroupGroup);
    ledGridWidget->setObjectName(QString::fromUtf8("LedGridWidget"));

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

    AnimationDetailsWidget* iAnimationDetailsWidget = new AnimationDetailsWidget(centralWidget, engine.animation());
    iAnimationDetailsWidget->setObjectName(QString::fromUtf8("AnimationDetailsWidget"));

    connect(ledGridWidget, SIGNAL(hideLed(int)), iAnimationDetailsWidget, SLOT(hideLed(int)));
    connect(ledGridWidget, SIGNAL(renumberLed(int,int,int)), iAnimationDetailsWidget, SLOT(renumberLed(int, int, int)));

    gridLayout->addWidget(iAnimationDetailsWidget, 2, 0, 1, 4);

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
    QAction* exportAction = fileMenu->addAction("&Export");
    fileMenu->addSeparator();
    QAction* quitAction = fileMenu->addAction("E&xit");
    quitAction->setShortcuts(QKeySequence::Quit);

    connect(newAction, SIGNAL(triggered()), &iEngine, SLOT(newAnimation()));
    connect(openAction, SIGNAL(triggered()), &iEngine, SLOT(loadAnimation()));
    connect(saveAction, SIGNAL(triggered()), &iEngine, SLOT(saveAnimation()));
    connect(saveAsAction, SIGNAL(triggered()), &iEngine, SLOT(saveAnimationAs()));
    connect(exportAction, SIGNAL(triggered()), &iEngine, SLOT(exportAnimation()));
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    menuBar()->addMenu(fileMenu);

    QMenu* editMenu = new QMenu("&Edit", this);
   /* QAction* cutAction = editMenu->addAction("&Cut");
    QAction* copyAction = editMenu->addAction("C&opy");
    QAction* pasteAction = editMenu->addAction("&Paste");*/

    // TODO make edit menu work

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

// events --------------------------------------

void MainWindow::closeEvent(QCloseEvent *event) {
    if (iEngine.askSaveAnimation()) {
        event->accept();
    } else {
        event->ignore();
    }
}

