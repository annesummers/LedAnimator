/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#include "MainWindow.h"

#include "Led.h"
#include "ledwidget.h"
#include "ledgridgroupwidget.h"
#include "ledgridwidget.h"
#include "TimeAxisPlayWidget.h"
#include "Engine.h"
#include "exceptions.h"
#include "Animation.h"
#include "TimeAxisDetailsWidget.h"
#include "ValueAxisDetailsWidget.h"
#include "grouplistwidget.h"
#include "LedDetailsWidget.h"
#include "FrameDetailsWidget.h"

#include "constants.h"

#include <QScrollArea>

using namespace AnimatorUi;
using namespace Exception;

MainWindow::MainWindow(Engine& engine) :
    iEngine(engine),
    iDeleteValueAxisMenu(NULL),
    iShowValueAxisMenu(NULL) {

    setWindowTitle(APP_NAME);

    iUndoStack = new QUndoStack(this);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameStyle(QFrame::Box);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    LedGridGroupWidget* ledGridGroupWidget = new LedGridGroupWidget(scrollArea, engine);
    LedGridWidget* ledGridWidget = new LedGridWidget(scrollArea, engine.animation(), *ledGridGroupWidget);
    ledGridWidget->setObjectName(QString::fromUtf8("LedGridWidget"));

    QHBoxLayout *layout = new QHBoxLayout(scrollArea);

    scrollArea->setWidget(ledGridWidget);
    layout->addWidget(ledGridWidget);

    QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    scrollArea->setSizePolicy(sizePolicy);

    connect(ledGridWidget, SIGNAL(maxSizeChanged(QSize)), this, SLOT(handleMaxSize(QSize)));

    connect(&engine.animation(), SIGNAL(newLedAdded(int, int)), ledGridWidget, SLOT(addLed(int, int)));
    connect(&engine.animation(), SIGNAL(newSocketAdded(int, int)), ledGridWidget, SLOT(addSocket(int, int)));
    connect(&engine.animation(), SIGNAL(ledDeleted(int, int, int)), ledGridWidget, SLOT(ledDeleted(int, int, int)));
    connect(&engine.animation(), SIGNAL(ledMoved(int, int, int, int)), ledGridWidget, SLOT(ledMoved(int, int, int, int)));

    iLedDetailsWidget = new LedDetailsWidget(NULL);
    iLedDetailsWidget->setObjectName(QString::fromUtf8("ledDetailsWidget"));
    iLedDetailsWidget->show();
    connect(ledGridWidget, SIGNAL(currentLedChanged(int, QColor)), iLedDetailsWidget, SLOT(handleCurrentLedChanged(int,QColor)));
    connect(ledGridWidget, SIGNAL(currentPositionChanged(int,int)), iLedDetailsWidget, SLOT(handleCurrentPositionChanged(int,int)));

    setCentralWidget(scrollArea);

    connect(&engine.animation(), SIGNAL(timeAxisAdded()), this, SLOT(addTimeAxisDetails()));
    connect(&engine.animation(), SIGNAL(valueAxisAdded(int)), this, SLOT(addValueAxisDetails(int)));
    connect(&engine.animation(), SIGNAL(valueAxisDeleted(int)), this, SLOT(deleteValueAxisDetails(int)));

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
    connect(iCopyAction, SIGNAL(triggered()), ledGridGroupWidget, SLOT(copySelected()));
    connect(iPasteAction, SIGNAL(triggered()), ledGridGroupWidget, SLOT(paste()));
    connect(iPasteWrapAction, SIGNAL(triggered()), ledGridGroupWidget, SLOT(pasteWrap()));

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

    iAnimationMenu = new QMenu("&Animation", this);
    QAction* copyToClipboardAction = iAnimationMenu->addAction("&Copy to clipboard");
    connect(copyToClipboardAction, SIGNAL(triggered()), &(iEngine.animation()), SLOT(copyToClipboard()));

    iAnimationMenu->addSeparator();

    iAddTimeAxisAction = iAnimationMenu->addAction("Add &time axis");
    iAddValueAxisAction = iAnimationMenu->addAction("Add &value axis");

    connect(iAddTimeAxisAction, SIGNAL(triggered()), &iEngine, SLOT(addTimeAxis()));
    connect(iAddValueAxisAction, SIGNAL(triggered()), &iEngine, SLOT(addValueAxis()));

    iAddTimeAxisAction->setEnabled(false);
    iAddValueAxisAction->setEnabled(true);

    iAnimationMenu->addSeparator();

    QAction* setFrameRateAction = iAnimationMenu->addAction("Set &frame rate");
    connect(setFrameRateAction, SIGNAL(triggered()), &iEngine, SLOT(setFrameRate()));

    iAnimationMenu->addSeparator();

    iShowValueAxisSignalMapper = new QSignalMapper(this);
    connect(iShowValueAxisSignalMapper, SIGNAL(mapped(int)), this, SLOT(showValueAxisDetails(int)));

    iShowTimeAxisAction = iAnimationMenu->addAction(tr("Show t&ime axis"));
    connect(iShowTimeAxisAction, SIGNAL(triggered()), this, SLOT(showTimeAxisDetails()));

    iDeleteValueAxisSignalMapper = new QSignalMapper(this);
    connect(iDeleteValueAxisSignalMapper, SIGNAL(mapped(int)), &iEngine.animation(), SLOT(deleteValueAxis(int)));

    //QAction* addFramesAction = animationMenu->addAction("&Add frames");
    //QAction* setNumFramesAction = animationMenu->addAction("Set &number of frames");
    //connect(addFramesAction, SIGNAL(triggered()), &iEngine, SLOT(addFrames()));
    //connect(setNumFramesAction, SIGNAL(triggered()), &iEngine, SLOT(setNumFrames()));

    menuBar()->addMenu(iAnimationMenu);

    readSettings();
}

MainWindow::~MainWindow() {
    writeSettings();

    if(iTimeAxisMainWidget != NULL) {
        delete iTimeAxisMainWidget;
    }

    for(ValueAxisDetailsWidget* detailsWidget : iValueAxisDetailsWidgets) {
        delete detailsWidget;
    }

    delete iLedDetailsWidget;
}

void MainWindow::addTimeAxisDetails() {
    iTimeAxisMainWidget = new QWidget(NULL);
    iTimeAxisMainWidget->setWindowTitle(QString("Time Axis"));

    FrameDetailsWidget* frameDetailsWidget = new FrameDetailsWidget(iTimeAxisMainWidget);
    frameDetailsWidget->setObjectName(QString::fromUtf8("frameDetailsWidget"));

    connect(iEngine.animation().timeAxis(), SIGNAL(currentFrameChanged(int)), frameDetailsWidget, SLOT(currentFrameChanged(int)));
    connect(iEngine.animation().timeAxis(), SIGNAL(highValueChanged(int)), frameDetailsWidget, SLOT(highValueChanged(int)));
    //connect(&engine.animation(), SIGNAL(framesInserted(int,int)), frameDetailsWidget, SLOT(framesInserted(int, int)));

    frameDetailsWidget->highValueChanged(iEngine.animation().timeAxis()->highValue());

    TimeAxisPlayWidget* playInfoWidget = new TimeAxisPlayWidget(iTimeAxisMainWidget, iEngine.animation(), *iEngine.animation().timeAxis());
    playInfoWidget->setObjectName(QString::fromUtf8("PlayInfoWidget"));

    TimeAxisDetailsWidget* axisDetailsWidget = new TimeAxisDetailsWidget(iTimeAxisMainWidget, iEngine.animation(), *iEngine.animation().timeAxis(), iEngine);
    axisDetailsWidget->setObjectName(QString::fromUtf8("TimeAxisDetailsWidget"));

    QGridLayout* gridLayout = new QGridLayout();
    gridLayout->setObjectName(QString::fromUtf8("verticalLayout"));

    gridLayout->addWidget(playInfoWidget, 0, 0, 1, 1);
    gridLayout->addWidget(frameDetailsWidget, 0, 1, 1, 1);
    gridLayout->addWidget(axisDetailsWidget, 1, 0, 1, 2);

    QHBoxLayout* mainLayout = new QHBoxLayout(iTimeAxisMainWidget);
    mainLayout->setObjectName(QString::fromUtf8("mainLayout"));

    mainLayout->addLayout(gridLayout);

    connect(&iEngine.animation(), SIGNAL(ledDeleted(int, int, int)), axisDetailsWidget, SLOT(ledDeleted(int, int, int)));
    connect(&iEngine.animation(), SIGNAL(ledRenumbered(int,int,int)), axisDetailsWidget, SLOT(ledRenumbered(int, int, int)));
    connect(iEngine.animation().timeAxis(), SIGNAL(currentFrameChanged(int)), axisDetailsWidget, SLOT(currentFrameChanged(int)));

    connect(iCutAction, SIGNAL(triggered()), axisDetailsWidget, SLOT(cutSelected()));
    connect(iCopyAction, SIGNAL(triggered()), axisDetailsWidget, SLOT(copySelected()));
    connect(iPasteAction, SIGNAL(triggered()), axisDetailsWidget, SLOT(paste()));
    connect(iPasteWrapAction, SIGNAL(triggered()), axisDetailsWidget, SLOT(pasteWrap()));
   // connect(&engine.animation(), SIGNAL(framesInserted(int,int)), animationDetailsWidget, SLOT(framesInserted(int, int)));

    showTimeAxisDetails();
}

void MainWindow::showTimeAxisDetails() {
    iTimeAxisMainWidget->show();
}

void MainWindow::hideTimeAxisDetails() {
    iTimeAxisMainWidget->hide();
}

void MainWindow::addValueAxisDetails(int axisNumber) {
    if(axisNumber != iValueAxisDetailsWidgets.size()) {
        throw InvalidAxisNumberException("WRONG");
    }

    ValueAxisDetailsWidget* axisDetailsWidget = new ValueAxisDetailsWidget(NULL, iEngine.animation(), iEngine.animation().axisAt(axisNumber), iEngine);

    iValueAxisDetailsWidgets.append(axisDetailsWidget);

    axisDetailsWidget->setObjectName(QString::fromUtf8("AnimationDetailsWidget"));
    axisDetailsWidget->setWindowTitle(QString("Value Axis %1").arg(axisNumber));

    if(iDeleteValueAxisMenu == NULL) {
        iDeleteValueAxisMenu = iAnimationMenu->addMenu("D&elete value axis");
    }

    connect(&iEngine.animation(), SIGNAL(ledDeleted(int, int, int)), axisDetailsWidget, SLOT(ledDeleted(int, int, int)));
    connect(&iEngine.animation(), SIGNAL(ledRenumbered(int,int,int)), axisDetailsWidget, SLOT(ledRenumbered(int, int, int)));

    QAction* deleteValueAxisAction = iDeleteValueAxisMenu->addAction(QString("%1").arg(axisNumber));

    iDeleteValueAxisSignalMapper->setMapping(deleteValueAxisAction, axisNumber);
    connect(deleteValueAxisAction, SIGNAL(triggered()), iDeleteValueAxisSignalMapper, SLOT(map()));

    if(iShowValueAxisMenu == NULL) {
        iShowValueAxisMenu = iAnimationMenu->addMenu("S&how value axis");
    }

    QAction* showValueAxisAction = iShowValueAxisMenu->addAction(QString("%1").arg(axisNumber));

    iShowValueAxisSignalMapper->setMapping(showValueAxisAction, axisNumber);
    connect(showValueAxisAction, SIGNAL(triggered()), iShowValueAxisSignalMapper, SLOT(map()));

    connect(iCutAction, SIGNAL(triggered()), axisDetailsWidget, SLOT(cutSelected()));
    connect(iCopyAction, SIGNAL(triggered()), axisDetailsWidget, SLOT(copySelected()));
    connect(iPasteAction, SIGNAL(triggered()), axisDetailsWidget, SLOT(paste()));
    connect(iPasteWrapAction, SIGNAL(triggered()), axisDetailsWidget, SLOT(pasteWrap()));

    showValueAxisDetails(axisNumber);
}

void MainWindow::showValueAxisDetails(int axisNumber) {
    ValueAxisDetailsWidget* detailsWidget = iValueAxisDetailsWidgets.at(axisNumber);

    detailsWidget->show();
}

void MainWindow::hideValueAxisDetails(int axisNumber) {
    ValueAxisDetailsWidget* detailsWidget = iValueAxisDetailsWidgets.at(axisNumber);

    detailsWidget->hide();
}

void MainWindow::deleteValueAxisDetails(int axisNumber) {
    ValueAxisDetailsWidget* widget = iValueAxisDetailsWidgets.at(axisNumber);
    iValueAxisDetailsWidgets.removeAt(axisNumber);
    delete widget;

    if(iEngine.animation().numValueAxes() == 0) {
        delete iDeleteValueAxisMenu;
        iDeleteValueAxisMenu = NULL;

        delete iShowValueAxisMenu;
        iShowValueAxisMenu = NULL;
    } else {
        QList<QAction*> deleteActions = iDeleteValueAxisMenu->actions();
        QList<QAction*> showActions = iShowValueAxisMenu->actions();
        for(int i = 0; i < deleteActions.size(); i++) {
            QAction* deleteAction = deleteActions.at(i);
            iDeleteValueAxisMenu->removeAction(deleteAction);
            iDeleteValueAxisSignalMapper->removeMappings(deleteAction);

            QAction* showAction = showActions.at(i);
            iShowValueAxisMenu->removeAction(showAction);
            iShowValueAxisSignalMapper->removeMappings(showAction);
        }

        for(int i = 0; i < iEngine.animation().numValueAxes(); i++) {
            QAction* deleteValueAxisAction = iDeleteValueAxisMenu->addAction(QString("%1").arg(i));
            iDeleteValueAxisSignalMapper->setMapping(deleteValueAxisAction, i);
            connect(deleteValueAxisAction, SIGNAL(triggered()), iDeleteValueAxisSignalMapper, SLOT(map()));

            QAction* showValueAxisAction = iShowValueAxisMenu->addAction(QString("%1").arg(i));
            iShowValueAxisSignalMapper->setMapping(showValueAxisAction, i);
            connect(showValueAxisAction, SIGNAL(triggered()), iShowValueAxisSignalMapper, SLOT(map()));

            iValueAxisDetailsWidgets.at(i)->setWindowTitle(QString("Value Axis %1").arg(i));
        }
    }
}

void MainWindow::handleMaxSize(QSize maximumSize) {
    setMaximumHeight(maximumSize.height());
    setMaximumWidth(maximumSize.width());

    resize(iPreferredSize);
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

    iPreferredSize = settings.value("size", QSize(600, 300)).toSize();
    resize(iPreferredSize);

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

