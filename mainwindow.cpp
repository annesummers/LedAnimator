/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include "mainwindow.h"

#include "led.h"
#include "ledwidget.h"
#include "ledgridwidget.h"
#include "playinfowidget.h"
#include "engine.h"
#include "animation.h"
#include "animationdetailswidget.h"
#include "grouplistwidget.h"

#include "constants.h"

using namespace AnimatorUi;

MainWindow::MainWindow(Engine& engine) :
    iEngine(engine) {

    setWindowTitle(APP_NAME);

    readSettings();

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setObjectName(QString::fromUtf8("centralWidget"));

    QGridLayout* gridLayout = new QGridLayout(centralWidget);
    gridLayout->setSpacing(6);
    gridLayout->setContentsMargins(11, 11, 11, 11);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));

    ColourGroupGroupWidget* gridGroupGroup = new ColourGroupGroupWidget(centralWidget);
    LedGridWidget* ledGridWidget = new LedGridWidget(centralWidget, engine.animation(), *gridGroupGroup);
    ledGridWidget->setObjectName(QString::fromUtf8("LedGridWidget"));

    gridLayout->addWidget(ledGridWidget, 0, 0, 2, 1);

    PlayInfoWidget* playInfoWidget = new PlayInfoWidget(centralWidget, engine.animation());
    playInfoWidget->setObjectName(QString::fromUtf8("PlayInfoWidget"));

    gridLayout->addWidget(playInfoWidget, 0, 1, 1, 1);

    QWidget* widget_4 = new QWidget(centralWidget);
    widget_4->setObjectName(QString::fromUtf8("widget_4"));

    gridLayout->addWidget(widget_4, 0, 2, 2, 1);

    GroupListWidget* groupListWidget = new GroupListWidget(centralWidget, engine.animation());
    groupListWidget->setObjectName(QString::fromUtf8("GroupListWidget"));

    gridLayout->addWidget(groupListWidget, 1, 1, 1, 1);

    AnimationDetailsWidget* iAnimationDetailsWidget = new AnimationDetailsWidget(centralWidget, engine.animation());
    iAnimationDetailsWidget->setObjectName(QString::fromUtf8("AnimationDetailsWidget"));

    connect(ledGridWidget, SIGNAL(hideLed(int, int)), iAnimationDetailsWidget, SLOT(deleteLed(int, int)));
    connect(ledGridWidget, SIGNAL(renumberLed(int,int,int)), iAnimationDetailsWidget, SLOT(renumberLed(int, int, int)));

    gridLayout->addWidget(iAnimationDetailsWidget, 2, 0, 1, 3);

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

    /*QMenu* editMenu = new QMenu("&Edit", this);
    QAction* cutAction = editMenu->addAction("&Cut");
    QAction* copyAction = editMenu->addAction("C&opy");
    QAction* pasteAction = editMenu->addAction("&Paste");

    // TODO make edit menu work

    menuBar()->addMenu(editMenu);*/

    QMenu* animationMenu = new QMenu("&Animation", this);
    QAction* copyToClipboardAction = animationMenu->addAction("&Copy to clipboard");
    QAction* setNumFramesAction = animationMenu->addAction("Set &number of frames");
    QAction* setFrameFrequencyAction = animationMenu->addAction("Set &frame frequency");

    connect(copyToClipboardAction, SIGNAL(triggered()), &(iEngine.animation()), SLOT(copyToClipboard()));
    connect(setNumFramesAction, SIGNAL(triggered()), &iEngine, SLOT(setNumFrames()));
    connect(setFrameFrequencyAction, SIGNAL(triggered()), &iEngine, SLOT(setFrameFrequency()));

    menuBar()->addMenu(animationMenu);

    QMenu* viewMenu = new QMenu("&View", this);
    QAction* toggleLedNumbersAction = viewMenu->addAction("&Toggle led numbers");

    connect(toggleLedNumbersAction, SIGNAL(triggered()), ledGridWidget, SLOT(toggleLedNumbers()));
// TODO attach this up
    menuBar()->addMenu(viewMenu);
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

