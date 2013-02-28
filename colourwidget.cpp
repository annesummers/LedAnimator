/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include "colourwidget.h"

#include <QtGui>
#include <QApplication>

#include "selectable.h"
#include "colourgroupwidget.h"

using namespace AnimatorUi;

ColourWidget::ColourWidget(QWidget* parent, ColourGroupWidget &groupWidget, Selectable& item) :
    SelectableWidget(parent, groupWidget, item),
    iFading(false),
    iColourDialog(NULL){

    iSetColourAction = new QAction(tr("&Set colour..."), this);
    iSetColourAction->setStatusTip(tr("Choose a colour"));

    connect(iSetColourAction, SIGNAL(triggered()), this, SLOT(chooseColour()));

  /*  iSetAllFramesColourAction = new QAction(tr("&Set colour for all frames..."), this);
    iSetAllFramesColourAction->setStatusTip(tr("Choose a colour"));

    connect(iSetAllFramesColourAction, SIGNAL(triggered()), this, SLOT(setAllFramesColour()));
*/

    // TODO set all frames colour
    iCopyAction = new QAction(tr("&Copy"), this);
    iCopyAction->setStatusTip(tr("Copy this colour"));

    connect(iCopyAction, SIGNAL(triggered()), this, SLOT(copy()));

    iFadeAction = new QAction(tr("&Fade"), this);
    iFadeAction->setStatusTip("Fade to last selected colour");

    connect(iFadeAction, SIGNAL(triggered()), this, SLOT(fade()));

    iFadeToAction = new QAction(tr("&Fade to..."), this);
    iFadeToAction->setStatusTip("Fade to chosen colour");

    connect(iFadeToAction, SIGNAL(triggered()), this, SLOT(fadeTo()));

    connect(&item, SIGNAL(selected()), this, SLOT(selected()));

  //  iColourDialog = new QColorDialog(Qt::white, this);
  //  iColourDialog->setOptions(QColorDialog::DontUseNativeDialog);

}

void ColourWidget::chooseColour() {
    connect(&colourGroup().colourDialog(), SIGNAL(accepted()), this, SLOT(colourDialogAccepted()));

    colourGroup().colourDialog().setCurrentColor(colour());
    colourGroup().colourDialog().exec();
}

/*void ColourWidget::setAllFramesColour() {
    iSetAllFrames = true;
    iColourDialog->exec();
}*/

void ColourWidget::colourDialogAccepted() {
    QColor newColour = colourGroup().colourDialog().currentColor();
    if(iFading) {
        if(newColour.isValid()) {
            colourGroup().fadeTo(newColour);

            iFading = false;
        }
    } /*else if(iSetAllFrames) {

        iSetAllFrames = false;
    }*/ else {
        colourGroup().setColour(newColour);
    }

    disconnect(&colourGroup().colourDialog());
}

void ColourWidget::updated() {
    setToolTip(QString("h: %1\ns: %2\nv: %3")
               .arg(colour().hue())
               .arg(colour().saturation())
               .arg(colour().value()));
    update();
}

void ColourWidget::fade() {
    if(!colourGroup().isGroupSelected()) {
        return;
    }

    colourGroup().fade();
}

void ColourWidget::fadeTo() {
    if(!colourGroup().isGroupSelected()) {
        return;
    }

    iFading = true;

    chooseColour();
}

// copy and paste ------------------------

void ColourWidget::copy() {
    const QClipboard *clipboard = QApplication::clipboard();

    if(clipboard->mimeData()->hasFormat(mimeType())) {
        colourGroup().handleOldMimeData(clipboard->mimeData()->data(mimeType()));
    }

    QApplication::clipboard()->setMimeData(mimeData());
}

// events -----------------------------------

void ColourWidget::contextMenuEvent(QContextMenuEvent *event) {
    SelectableWidget::contextMenuEvent(event);

    QMenu menu(this);

    menu.addAction(iSetColourAction);
    //menu.addAction(iSetAllFramesColourAction);
    addExtraActions(&menu);
    menu.addSeparator();
    menu.addAction(iFadeAction);
    menu.addAction(iFadeToAction);
    menu.addSeparator();
    addCutAction(&menu);
    menu.addAction(iCopyAction);

    addPasteActions(&menu);

    menu.exec(event->globalPos());
}

void ColourWidget::mouseDoubleClickEvent(QMouseEvent* event) {
    SelectableWidget::mouseDoubleClickEvent(event);

    chooseColour();
}

void ColourWidget::keyPressEvent(QKeyEvent *event) {
    Qt::Key key = (Qt::Key)event->key();

    if(key == Qt::Key_C && event->modifiers() == Qt::ControlModifier) {
        copy();
    } else if(key == Qt::Key_X && event->modifiers() == Qt::ControlModifier) {
        cut();
    } else if (key == Qt::Key_V && event->modifiers() == Qt::ControlModifier) {
        paste(false);
    }
}
