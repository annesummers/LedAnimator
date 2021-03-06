/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#include "engine.h"

#include "Animation.h"
#include "MainWindow.h"
#include "constants.h"
#include "ByteArrayCodec.h"
#include "NewAnimationDialog.h"
#include "NewValueAxisDialog.h"
#include "NewTimeAxisDialog.h"
#include "importbitmap.h"
#include "exceptions.h"


#include <QFile>
#include <QFileDialog>
#include <QSettings>
#include <QCoreApplication>
#include <QMessageBox>
#include <QInputDialog>
#include <QClipboard>
#include <QStandardPaths>

using namespace ImportExport;
using namespace Exception;

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
    } else if(!doLoad(animFileName)) {
        newAnimation(false);
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

void Engine::setupNewAnimation(int numRows, int numColumns) {
    iAnimation->newAnimation(numRows, numColumns);

    addTimeAxis();

    iMainWindow->show();
}

bool Engine::doLoad(QString fileName) {
    QFile file(fileName);

    if(file.open(QIODevice::ReadOnly)) {
        QByteArray anim = file.readAll();
        file.close();

        LedAnimByteArrayCodec codec(*iAnimation, anim);
        try {
            codec.readHeader();

            setupUI();

            codec.readAnimation();

            iAnimation->setFileName(fileName);

            QSettings settings;
            settings.setValue(SETTINGS_USER_CURRENT_ANIM, fileName);

            //fileName = fileName.right(fileName.size() - fileName.lastIndexOf("/") - 1);

            iMainWindow->setWindowTitle(fileName);
            iMainWindow->show();

        } catch(InvalidFileException exception) {
            QMessageBox::critical(iMainWindow,
                                  "Error Loading Animation",
                                  exception.errorMessage());

            return false;
        }

        return true;
    }

    return false;
}

void Engine::doSave(QString fileName) {
    LedAnimByteArrayCodec codec(*iAnimation);
    codec.writeAnimation(true);

    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    file.write(codec.asByteArray());
    file.close();

    QSettings settings;
    settings.setValue(SETTINGS_USER_CURRENT_ANIM, fileName);
}

void Engine::doExport(QString fileName) {
    LedAnimSimpleByteArrayCodec codec(*iAnimation);
    codec.writeAnimation(false);

    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    file.write(codec.asByteArray());
    file.close();
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
        QSettings settings;

        QVariant var = settings.value(SETTINGS_USER_OPEN_PATH, "");
        QString loadPath = var.toString();

        if(loadPath == "") {
            loadPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
        }

        QFileDialog* fileDialog = new QFileDialog(iMainWindow, "Open Animation", loadPath);

        QStringList filters;
        filters << "Led Animator files (*.laan)"
                << "All files (*)";

        fileDialog->setNameFilters(filters);
        fileDialog->setFileMode(QFileDialog::ExistingFile);
        fileDialog->setAcceptMode(QFileDialog::AcceptOpen);

        if(fileDialog->exec() == QDialog::Accepted) {
            QString fileName = "";

            QStringList fileNames = fileDialog->selectedFiles();
            fileName = fileNames.takeFirst();

            settings.setValue(SETTINGS_USER_OPEN_PATH, fileDialog->directory().absolutePath());

            delete fileDialog;

            doLoad(fileName);
        }
    }
}

void Engine::exportAnimation() {
    if(iAnimation->ledsMissing()) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(iMainWindow, tr("Animation is missing leds"),
                                    tr("There are leds missing from the animation.  Do you want to continue the export?"),
                                    QMessageBox::Yes | QMessageBox::No);
        if (reply != QMessageBox::Yes) {
            return;
        }
    }

    QSettings settings;

    QVariant var = settings.value(SETTINGS_USER_EXPORT_PATH, "");
    QString exportPath = var.toString();

    if(exportPath == "") {
        exportPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    }

    QFileDialog fileDialog(iMainWindow, "Export Animation", exportPath);

    QStringList filters;
    filters << "Led Animator export Files (*.lax)"
            << "All files (*)";

    fileDialog.setNameFilters(filters);
    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);

    if(fileDialog.exec() == QDialog::Accepted) {
        QString fileName = "";

        QStringList fileNames = fileDialog.selectedFiles();
        fileName = fileNames.takeFirst();

        settings.setValue(SETTINGS_USER_EXPORT_PATH, fileDialog.directory().absolutePath());

        if(fileName != "") {
            doExport(fileName);
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
    QSettings settings;

    QVariant var = settings.value(SETTINGS_USER_SAVE_PATH, "");
    QString savePath = var.toString();

    if(savePath == "") {
        savePath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    }

    QFileDialog fileDialog(iMainWindow, "Save Animation");

    QStringList filters;
    filters << "Led Animator files (*.laan)"
            << "All files (*)";

    fileDialog.setNameFilters(filters);
    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setDirectory(QDir(savePath));

    if(fileDialog.exec() == QDialog::Accepted) {
        QString fileName = "";

        QStringList fileNames = fileDialog.selectedFiles();
        fileName = fileNames.takeFirst();

        settings.setValue(SETTINGS_USER_SAVE_PATH, fileDialog.directory().absolutePath());

        if(fileName != "") {
            doSave(fileName);
        }
    }
}

void Engine::quit() {
    iMainWindow->close();
    emit doQuit();
}

void Engine::setFrameRate() {
    bool ok;
    int rate = QInputDialog::getInt(iMainWindow, tr("Choose time axis frame rate"),
                                  tr("Frame rate (ms):"),
                                    iAnimation->timeAxis()->frameRate(),
                                    MIN_FRAME_RATE,
                                    MAX_FRAME_RATE,
                                    1, &ok);
    if (ok) {
        iAnimation->timeAxis()->setFrameRate(rate);
    }
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
}*/

void Engine::addTimeAxis() {
    NewTimeAxisDialog newTimeAxisDialog(NULL, *this);
    if(newTimeAxisDialog.exec() == QDialog::Accepted) {
        iAnimation->addTimeAxis(0,
                                newTimeAxisDialog.iNumFrames - 1,
                                newTimeAxisDialog.iFrameRate,
                                newTimeAxisDialog.iPriority,
                                newTimeAxisDialog.iOpaque);
    }
}

void Engine::addValueAxis() {
    NewValueAxisDialog newValueAxisDialog(NULL, *this);
    if(newValueAxisDialog.exec() == QDialog::Accepted) {
        iAnimation->addValueAxis(newValueAxisDialog.iLowValue,
                                 newValueAxisDialog.iHighValue,
                                 newValueAxisDialog.iZeroValue,
                                 newValueAxisDialog.iPriority,
                                 newValueAxisDialog.iOpaque);
    }
}

void Engine::importBitmap() {
    QString fileName = QFileDialog::getOpenFileName(iMainWindow,
                                                    "Import bitmap",
                                                    QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                                                    "PNG (*.png)");

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
