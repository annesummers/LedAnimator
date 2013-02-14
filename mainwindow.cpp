/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include "mainwindow.h"

//#include "ui_mainwindow.h"
#include "led.h"
#include "ledwidget.h"
#include "ledgridwidget.h"
#include "playinfowidget.h"
#include "engine.h"
#include "animation.h"
#include "animationdetailswidgets.h"

#include "defaults.h"

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

    LedGridWidget* iLedGridWidget = new LedGridWidget(centralWidget, engine.animation());
    iLedGridWidget->setObjectName(QString::fromUtf8("iLedGridWidget"));

    gridLayout->addWidget(iLedGridWidget, 0, 0, 2, 1);

    PlayInfoWidget* iPlayInfoWidget = new PlayInfoWidget(centralWidget, engine.animation());
    iPlayInfoWidget->setObjectName(QString::fromUtf8("iPlayInfoWidget"));

    gridLayout->addWidget(iPlayInfoWidget, 0, 1, 1, 1);

    QWidget* widget_4 = new QWidget(centralWidget);
    widget_4->setObjectName(QString::fromUtf8("widget_4"));

    gridLayout->addWidget(widget_4, 0, 2, 1, 1);

    QWidget* widget_3 = new QWidget(centralWidget);
    widget_3->setObjectName(QString::fromUtf8("widget_3"));

    gridLayout->addWidget(widget_3, 1, 1, 1, 2);

    AnimationDetailsWidget* iAnimationDetailsWidget = new AnimationDetailsWidget(centralWidget, engine.animation());
    iAnimationDetailsWidget->setObjectName(QString::fromUtf8("iAnimationDetailsWidget"));

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
    QAction* quitAction = fileMenu->addAction("E&xit");
    quitAction->setShortcuts(QKeySequence::Quit);

    connect(newAction, SIGNAL(triggered()), &iEngine, SLOT(newAnimation()));
    connect(openAction, SIGNAL(triggered()), &iEngine, SLOT(loadAnimation()));
    connect(saveAction, SIGNAL(triggered()), &iEngine, SLOT(saveAnimation()));
    connect(saveAsAction, SIGNAL(triggered()), &iEngine, SLOT(saveAnimationAs()));
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    menuBar()->addMenu(fileMenu);

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
        //writeSettings(); // this is done in the destructor now
        event->accept();
    } else {
        event->ignore();
    }
}

