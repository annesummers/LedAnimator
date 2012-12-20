#include "mainwindow.h"

#include "ui_mainwindow.h"
#include "led.h"
#include "ledwidgets.h"
#include "ledgridwidget.h"
#include "playinfowidget.h"
#include "engine.h"
#include "animation.h"
#include "animationdetailswidgets.h"

#include "defaults.h"

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

    QSplitter* splitterVertical = new QSplitter(this);
    splitterVertical->setOrientation(Qt::Vertical);
    setCentralWidget(splitterVertical);

    QSplitter* splitterHorizontal = new QSplitter(splitterVertical);
    splitterHorizontal->setOrientation(Qt::Horizontal);

    LedGridContainerWidget* gridContainer = new LedGridContainerWidget(splitterHorizontal);
    LedGridWidget* ledGridWidget = new LedGridWidget(gridContainer, animation());
    ledGridWidget->move(LED_GRID_BORDER, LED_GRID_BORDER);

    PlayInfoWidget* playInfoWidget = new PlayInfoWidget(splitterHorizontal, animation());

    splitterHorizontal->addWidget(gridContainer);
    splitterHorizontal->addWidget(playInfoWidget);

     QWidget* animationDetailsContainer = new QWidget(splitterVertical);
    AnimationDetailsWidget* animationDetailsWidget = new AnimationDetailsWidget(animationDetailsContainer, animation());//animationDetailsContainer->widget();

    animationDetailsWidget->move(LED_GRID_BORDER, 0);
    animationDetailsWidget->resize(animationDetailsContainer->width() - LED_GRID_BORDER*2, animationDetailsContainer->height());

    splitterVertical->addWidget(splitterHorizontal);
    splitterVertical->addWidget(animationDetailsContainer);

    QObject::connect(&animation(), SIGNAL(numFramesChanged(int)), animationDetailsWidget, SLOT(numFramesChanged(int)));

    QObject::connect(&animation(), SIGNAL(currentFrameChanged(int)), playInfoWidget, SLOT(currentFrameChanged(int)));
    QObject::connect(&animation(), SIGNAL(currentFrameChanged(int)), animationDetailsWidget, SLOT(currentFrameChanged(int)));

    QObject::connect(&animation(), SIGNAL(newLed(Led&)), ledGridWidget, SLOT(addLed(Led&)));
    QObject::connect(&animation(), SIGNAL(newLed(Led&)), animationDetailsWidget, SLOT(newLed(Led&)));
}

Animation& MainWindow::animation() {
    return iEngine.animation();
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
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

// -------------------------------------------

