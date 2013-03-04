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
    iSignalMapper(NULL),
    iSetColourAction(NULL),
    iFadeAction(NULL),
    iFadeToAction(NULL),
    iCopyAction(NULL),
    iFading(false) {

    iSetColourAction = new QAction(tr("&Set colour..."), this);
    iSetColourAction->setStatusTip(tr("Choose a colour"));

    connect(iSetColourAction, SIGNAL(triggered()), this, SLOT(chooseColour()));

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

    iSignalMapper = new QSignalMapper(this);
    iSignalMapper->setMapping(&colourGroup().colourDialog(), iItem.number());
    connect(&colourGroup().colourDialog(), SIGNAL(accepted()), iSignalMapper, SLOT(map()));
}

void ColourWidget::chooseColour() {
    connect(iSignalMapper, SIGNAL(mapped(int)), this, SLOT(colourDialogAccepted(int)));
    colourGroup().colourDialog().setCurrentColor(colour());
    colourGroup().colourDialog().exec();
}

void ColourWidget::colourDialogAccepted(int number) {
    if(number == iItem.number()) {
        QColor newColour = colourGroup().colourDialog().currentColor();
        if(iFading) {
            colourGroup().fadeTo(newColour);

            iFading = false;
        } else {
            colourGroup().setColour(newColour);
        }
    }

    iSignalMapper->disconnect(this);
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
    addExtraActions(&menu);

    if(colourGroup().isGroupSelected()) {
        menu.addSeparator();
        menu.addAction(iFadeAction);
        menu.addAction(iFadeToAction);
    }

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
