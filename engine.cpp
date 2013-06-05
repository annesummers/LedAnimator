/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#include "engine.h"

#include "animation.h"
#include "mainwindow.h"
#include "constants.h"
#include "ledanimcodec.h"
#include "newanimationdialog.h"

#include <QFile>
#include <QFileDialog>
#include <QSettings>
#include <QCoreApplication>
#include <QMessageBox>
#include <QInputDialog>

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

void Engine::setupNewAnimation(int numRows, int numColumns, int numFrames, int frameFrequency) {
    iAnimation->setupNew(numRows, numColumns, numFrames, frameFrequency);
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
                                 newAnimationDialog.iNumColumns,
                                 newAnimationDialog.iNumFrames,
                                 newAnimationDialog.iFrameFrequency);
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

void Engine::setNumFrames() {
    bool ok;
    int i = QInputDialog::getInt(iMainWindow, tr("Choose animation number of frames"),
                                  tr("Number of frames:"), 1, 1, 1000, 1, &ok);
    if (ok) {
        iAnimation->setNumFrames(i);
    }
}

void Engine::setFrameFrequency() {
    bool ok;
    int i = QInputDialog::getInt(iMainWindow, tr("Choose animation frame frequency"),
                                  tr("Frame frequency (ms):"), 1000, 1, 10000, 1, &ok);
    if (ok) {
        iAnimation->setFrameFrequency(i);
    }
}

void Engine::importBitmap() {

}
