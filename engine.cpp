/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#include "engine.h"

#include "Animation.h"
#include "mainwindow.h"
#include "constants.h"
#include "Codec.h"
#include "newanimationdialog.h"
#include "importbitmap.h"

#include <QFile>
#include <QFileDialog>
#include <QSettings>
#include <QCoreApplication>
#include <QMessageBox>
#include <QInputDialog>
#include <QClipboard>

using namespace ImportExport;

Engine::Engine(QObject *parent) :
    QObject(parent),
    iMainWindow(NULL),
    iAnimation(NULL) {
}

void Engine::start() {
    QCoreApplication::setOrganizationName(COMPANY_NAME);
    QCoreApplication::setOrganizationDomain(COMPANY_DOMAIN);
    QCoreApplication::setApplicationName(APP_NAME);

    connect(this, SIGNAL(doQuit()), qApp, SLOT(quit()));

    iAnimation = new Animation(*this);

    QSettings settings;
    QVariant var = settings.value(SETTINGS_USER_CURRENT_ANIM, "");
    QString animFileName = var.toString();

    if(animFileName == "") {
        newAnimation(false);
    } else {
        if(!doLoad(animFileName)) {
            newAnimation(false);
        }
    }
}

bool Engine::askSaveAnimation() {
    if(!iAnimation->isSaved()) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(iMainWindow, tr("Animation Not Saved"),
                                    tr("Do you want to save the current animation?"),
                                    QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (reply == QMessageBox::Yes) {
            saveAnimation();
            return true;
        } else if (reply == QMessageBox::No) {
            return true;
        } else {
            return false;
        }
    }

    return true;
}

void Engine::setupUI() {
    if(iMainWindow != NULL) {
        delete iMainWindow;
        iMainWindow = NULL;
    }

    iMainWindow = new MainWindow(*this);    
    iAnimation->setUndoStack(iMainWindow->undoStack());
}

void Engine::setupNewAnimation(int numRows, int numColumns ) {
    iAnimation->newAnimation(numRows, numColumns);
    iMainWindow->show();
}

bool Engine::doLoad(QString fileName) {
    QFile file(fileName);

    if(file.open(QIODevice::ReadOnly)) {
        QByteArray anim = file.readAll();
        file.close();

        setupUI();

        LedAnimByteArrayCodec codec(*iAnimation, anim);
        codec.readAnimation();

        iAnimation->setFileName(fileName);

        QSettings settings;
        settings.setValue(SETTINGS_USER_CURRENT_ANIM, fileName);
        iMainWindow->show();

        return true;
    }

    return false;
}

void Engine::doSave(QString fileName, bool withPositions) {
    LedAnimByteArrayCodec codec(*iAnimation);
    codec.writeAnimation(withPositions);

    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    file.write(codec.asByteArray());
    file.close();

    QSettings settings;
    settings.setValue(SETTINGS_USER_CURRENT_ANIM, fileName);
}

// slots -------------------

void Engine::newAnimation(bool askSaveOld) {
    if(!askSaveOld ||
        askSaveAnimation()) {
        NewAnimationDialog newAnimationDialog(NULL, *this);
        if(newAnimationDialog.exec() == QDialog::Accepted) {
            setupUI();

            setupNewAnimation(newAnimationDialog.iNumRows,
                                 newAnimationDialog.iNumColumns);
        }
    }
}

void Engine::loadAnimation() {
    if(askSaveAnimation()) {
        QString fileName = QFileDialog::getOpenFileName(iMainWindow, "Open Animation", "~", "Led Animation Files (*.anim)");

        if(fileName != "") {
            if(!doLoad(fileName)) {
                // TODO what if we don't find the file?
            }
        }
    }
}

void Engine::exportAnimation() {
    if(iAnimation->ledsMissing()) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(iMainWindow, tr("Animation is missing leds"),
                                    tr("There are leds missing from the animation.  Do you want to continue the export?"),
                                    QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            QString fileName = QFileDialog::getSaveFileName(iMainWindow, "Export Animation", "~", "Led Animation Files (*.anim)");

            if(fileName != "") {
                doSave(fileName);
            }
        }
    }
}

void Engine::saveAnimation() {
    if(iAnimation->fileName() == "") {
        saveAnimationAs();
    } else {
        doSave(iAnimation->fileName());
    }
}

void Engine::saveAnimationAs() {
    QString fileName = QFileDialog::getSaveFileName(iMainWindow, "Save Animation", "~", "Led Animation Files (*.anim)");

    if(fileName != "") {
        doSave(fileName);
    }
}

void Engine::quit() {
    iMainWindow->close();
    emit doQuit();
}

/*void Engine::addFrames() {
    bool ok;
    int numFrames = QInputDialog::getInt(iMainWindow, tr("Add how many frames?"),
                                  tr("Number of frames:"), 1, 1, 1000, 1, &ok);
    if (ok) {
        //iAnimation->addFrames(numFrames);
    }
}

void Engine::setNumFrames() {
    bool ok;
    int numFrames = QInputDialog::getInt(iMainWindow, tr("Choose animation number of frames"),
                                  tr("Number of frames:"), 1, 1, 1000, 1, &ok);
    if (ok) {
        //iAnimation->setNumFrames(numFrames);
    }
}

void Engine::setFrameFrequency() {
    bool ok;
    int frequency = QInputDialog::getInt(iMainWindow, tr("Choose animation frame frequency"),
                                  tr("Frame frequency (ms):"), 1000, 1, 10000, 1, &ok);
    if (ok) {
        //iAnimation->setFrameFrequency(frequency);
    }
}*/

void Engine::addTimeAxis() {
    iAnimation->addTimeAxis(0, 100, 100, kPriorityLow, false);
}

void Engine::addValueAxis() {
    iAnimation->addValueAxis(-100, 100, 0, kPriorityLow, false);
}

void Engine::importBitmap() {
    QString fileName = QFileDialog::getOpenFileName(iMainWindow, "Import bitmap", "~", "PNG (*.png)");

    if(fileName != "") {
        setupUI();

        ImportBitmap import(fileName, *iAnimation);

        import.doImport();

        iMainWindow->show();
    }
}

void Engine::setSelectedGroupGroup(SelectableGroupGroupWidget* groupGroup) {
    iSelectedGroupGroup = groupGroup;

    if(iMainWindow != NULL) {
        if(iSelectedGroupGroup == NULL) {
            iMainWindow->setEnabledCutAction(false);
            iMainWindow->setEnabledCopyAction(false);
            iMainWindow->setEnabledPasteActions(false);
        } else {
            iMainWindow->setEnabledCutAction(groupGroup->canCut());
            iMainWindow->setEnabledCopyAction(true);
            iMainWindow->setEnabledPasteActions(QApplication::clipboard()->mimeData()->hasFormat(groupGroup->mimeType()));
        }
    }
}
