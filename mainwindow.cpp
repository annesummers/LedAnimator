/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include "mainwindow.h"

#include "ui_mainwindow.h"
#include "led.h"
#include "ledwidget.h"
#include "ledgridwidget.h"
#include "playinfowidget.h"
#include "engine.h"
#include "animation.h"
#include "animationdetailswidgets.h"

#include "defaults.h"

using namespace Ui;

MainWindow::MainWindow(Engine& engine) :
    iEngine(engine){

    setWindowTitle(APP_NAME);

    readSettings();

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

    QSplitter* splitterVertical = new QSplitter(this);
    splitterVertical->setOrientation(Qt::Vertical);
    setCentralWidget(splitterVertical);

    QSplitter* splitterHorizontal = new QSplitter(splitterVertical);
    splitterHorizontal->setOrientation(Qt::Horizontal);

    LedGridWidget* ledGridWidget = new LedGridWidget(splitterHorizontal, engine.animation());
    ledGridWidget->move(BORDER, BORDER);

    PlayInfoWidget* playInfoWidget = new PlayInfoWidget(splitterHorizontal, engine.animation());

    splitterHorizontal->addWidget(ledGridWidget);
    splitterHorizontal->addWidget(playInfoWidget);

    QWidget* animationDetailsContainer = new QWidget(splitterVertical);
    AnimationDetailsWidget* animationDetailsWidget = new AnimationDetailsWidget(animationDetailsContainer, engine.animation());//animationDetailsContainer->widget();

    animationDetailsWidget->move(BORDER, 0);
    animationDetailsWidget->resize(animationDetailsContainer->width() - BORDER*2, animationDetailsContainer->height());

    splitterVertical->addWidget(splitterHorizontal);
    splitterVertical->addWidget(animationDetailsContainer);
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

