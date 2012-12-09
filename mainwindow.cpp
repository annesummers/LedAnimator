#include "mainwindow.h"

#include "ui_mainwindow.h"
#include "led.h"
#include "ledwidget.h"
#include "ledgridwidget.h"
#include "playinfowidget.h"
#include "engine.h"
#include "animation.h"
#include "framedetailswidget.h"

#include "defaults.h"

MainWindow::MainWindow(Engine& engine) :
    iEngine(engine),
    //iKittenWidget(NULL),
    //iSplitterHorizontal(NULL),
    //iSplitterVertical(NULL),
    iLedGridWidget(NULL),
    iPlayInfoWidget(NULL),
    iFrameDetailsWidget(NULL) {

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

    QSplitter* splitterVertical = new QSplitter(this);
    splitterVertical->setOrientation(Qt::Vertical);
    setCentralWidget(splitterVertical);

    QSplitter* splitterHorizontal = new QSplitter(splitterVertical);
    splitterHorizontal->setOrientation(Qt::Horizontal);

    iLedGridWidget = new LedGridContainerWidget(splitterHorizontal, *this);
    //iKittenWidget = new QWidget(iSplitterHorizontal);
    iPlayInfoWidget = new PlayInfoWidget(splitterHorizontal, *this);

    splitterHorizontal->addWidget(iLedGridWidget);
    splitterHorizontal->addWidget(iPlayInfoWidget);

    iFrameDetailsWidget = new FrameDetailsWidget(splitterVertical, *this);

    splitterVertical->addWidget(splitterHorizontal);
    splitterVertical->addWidget(iFrameDetailsWidget);

    QObject::connect(&(iEngine.animation()), SIGNAL(numFramesChanged(int)), iFrameDetailsWidget, SLOT(numFramesChanged(int)));

    QObject::connect(&(iEngine.animation()), SIGNAL(currentFrameChanged(int)), iLedGridWidget, SLOT(currentFrameChanged(int)));
    QObject::connect(&(iEngine.animation()), SIGNAL(currentFrameChanged(int)), iPlayInfoWidget, SLOT(currentFrameChanged(int)));
    QObject::connect(&(iEngine.animation()), SIGNAL(currentFrameChanged(int)), iFrameDetailsWidget, SLOT(currentFrameChanged(int)));

    QObject::connect(&(iEngine.animation()), SIGNAL(ledColourChanged(int, int, int)), iLedGridWidget, SLOT(ledColourChanged(int, int, int)));
    QObject::connect(&(iEngine.animation()), SIGNAL(ledColourChanged(int, int, int)), iFrameDetailsWidget, SLOT(ledColourChanged(int, int, int)));

    QObject::connect(&(iEngine.animation()), SIGNAL(newLed(Led&)), iLedGridWidget, SLOT(newLed(Led&)));
    QObject::connect(&(iEngine.animation()), SIGNAL(newLed(Led&)), iFrameDetailsWidget, SLOT(newLed(Led&)));
}

MainWindow::~MainWindow() {
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

void MainWindow::closeEvent(QCloseEvent *event) {
    if (iEngine.askSaveAnimation()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

// slots -----------------------------------------

void MainWindow::setCurrentFrame(int frame) {
    iEngine.animation().setCurrentFrame(frame);
}

void MainWindow::playClicked() {
    if(!iEngine.animation().isPlaying()) {
        iEngine.animation().play();
    } else {
        iEngine.animation().stop();
    }
}

// -----------------------------------------------
