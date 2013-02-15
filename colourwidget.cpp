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
    QWidget(parent),
    iColourGroup(groupWidget),
    iItem(item),
    iDoubleClick(false),
    iFading(false),
    iColourDialog(NULL){

    setAcceptDrops(true);

    iSetColourAction = new QAction(tr("&Set colour"), this);
    iSetColourAction->setStatusTip(tr("Choose a colour"));

    connect(iSetColourAction, SIGNAL(triggered()), this, SLOT(chooseColour()));

    iCopyAction = new QAction(tr("&Copy"), this);
    iCopyAction->setStatusTip(tr("Copy this colour"));

    connect(iCopyAction, SIGNAL(triggered()), this, SLOT(copy()));

    iPasteAction = new QAction(tr("&Paste"), this);
    iPasteAction->setStatusTip(tr("Paste the copied colour"));

    connect(iPasteAction, SIGNAL(triggered()), this, SLOT(paste()));

    iFadeAction = new QAction(tr("&Fade"), this);
    iFadeAction->setStatusTip("Fade to last selected colour");

    connect(iFadeAction, SIGNAL(triggered()), this, SLOT(fade()));

    iFadeToAction = new QAction(tr("&Fade to..."), this);
    iFadeToAction->setStatusTip("Fade to chosen colour");

    connect(iFadeToAction, SIGNAL(triggered()), this, SLOT(fadeTo()));

    connect(&item, SIGNAL(selected()), this, SLOT(selected()));

    iColourDialog = new QColorDialog(Qt::white, this);
    iColourDialog->setOptions(QColorDialog::DontUseNativeDialog);
    connect(iColourDialog, SIGNAL(accepted()), this, SLOT(colourDialogAccepted()));
}

void ColourWidget::chooseColour() {
    iColourDialog->exec();
}

void ColourWidget::colourDialogAccepted() {
    QColor newColour = iColourDialog->currentColor();
    if(iFading) {
        if(newColour.isValid()) {
            iColourGroup.fadeTo(newColour);

            iFading = false;
        }
    } else {
        iColourGroup.setColour(newColour);
    }
}

void ColourWidget::updated() {
    setToolTip(QString("h: %1\ns: %2\nv: %3")
               .arg(colour().hue())
               .arg(colour().saturation())
               .arg(colour().value()));
    update();
}

void ColourWidget::fade() {
    if(!iColourGroup.isGroupSelected()) {
        return;
    }

    iColourGroup.fade();
}

void ColourWidget::fadeTo() {
    if(!iColourGroup.isGroupSelected()) {
        return;
    }

    iFading = true;

    chooseColour();
}

QMimeData* ColourWidget::mimeData() {
    QMimeData *data = new QMimeData;
    data->setData(mimeType(), iColourGroup.mimeData());

    return data;
}

// copy and paste ------------------------

void ColourWidget::copy() {
    QApplication::clipboard()->setMimeData(mimeData());
}

void ColourWidget::paste(bool wrap) {
    const QClipboard *clipboard = QApplication::clipboard();

    if(clipboard->mimeData()->hasFormat(mimeType())) {
        iColourGroup.handleMimeData(clipboard->mimeData()->data(mimeType()), *this, wrap);
    }
}

// events ----------------------------------------

void ColourWidget::mousePressEvent(QMouseEvent* event) {
    //qDebug("singleWidget mousePress");
    if (event->button() != Qt::LeftButton) {
        return;
    }

    iDragStartPosition = event->pos();

    if((QApplication::keyboardModifiers() & Qt::ControlModifier) != 0 &&
         !iColourGroup.isGroupSelected()) {
         iColourGroup.toggle(*this);
         return;
    }
}

void ColourWidget::mouseReleaseEvent(QMouseEvent* event){
    //qDebug("singleWidget mouseRelease");
    if (event->button() != Qt::LeftButton ||
       (QApplication::keyboardModifiers() & Qt::ControlModifier) != 0) {
        return;
    }

    if((QApplication::keyboardModifiers() & Qt::ShiftModifier) != 0) {
        iColourGroup.selectArea(*this);
        return;
    }

    if(!iDoubleClick) {
        iColourGroup.toggleOne(*this);
    } else {
        iDoubleClick = false;
    }
}

void ColourWidget::mouseMoveEvent(QMouseEvent *event) {
    //qDebug("singelWidge mouse move");
    if(event->buttons() != Qt::LeftButton ||
       //iColourGroup.isMultipleSelected() ||
       (event->pos() - iDragStartPosition).manhattanLength() < QApplication::startDragDistance()) {
        return;
    }

    if(!iColourGroup.isGroupSelected() &&
       !iColourGroup.isMultipleSelected()) {
        iColourGroup.selectOne(*this);
    }

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData());
    drag->setHotSpot(pos());

    if(iColourGroup.isMultipleSelected()) {
        drag->exec(dropAction());
    } else {
        drag->exec(Qt::CopyAction | dropAction());
    }
}

void ColourWidget::mouseDoubleClickEvent(QMouseEvent* event) {
   // qDebug("singleWidget mouseDoubleClick");
    if (event->buttons() != Qt::LeftButton) {
        return;
    }

    iDoubleClick = true;
    iColourGroup.selectOne(*this);

    chooseColour();
}

void ColourWidget::keyPressEvent(QKeyEvent *event) {
    Qt::Key key = (Qt::Key)event->key();

    if(key == Qt::Key_C && event->modifiers() == Qt::ControlModifier) {
        copy();
    } else if (key == Qt::Key_V && event->modifiers() == Qt::ControlModifier) {
        paste(false);
    }
}

void ColourWidget::contextMenuEvent(QContextMenuEvent *event) {
    if(!iColourGroup.isAnySelected()) {
        iColourGroup.selectOne(*this);
    }

    QMenu menu(this);

    menu.addAction(iSetColourAction);
    menu.addAction(iFadeAction);
    menu.addAction(iCopyAction);

    const QClipboard *clipboard = QApplication::clipboard();
    if(clipboard->mimeData()->hasFormat(mimeType())) {
        menu.addAction(iPasteAction);
    }

    menu.exec(event->globalPos());
}

void ColourWidget::dragEnterEvent(QDragEnterEvent* event) {
   // qDebug("singleWidget drag enter");
    handleDragMoves(event);
}

bool ColourWidget::handleDragMoves(QDropEvent* event) {
    if (event->mimeData()->hasFormat(mimeType())) {
        if (event->source() != 0 &&
            event->source() != this &&
            event->proposedAction() == Qt::CopyAction) {
            event->accept();

            return true;
         }
     } else {
         event->ignore();

         return false;
     }
}

void ColourWidget::dragMoveEvent(QDragMoveEvent* event) {
   // qDebug("singleWidget drag move");
    handleDragMoves(event);
}

void ColourWidget::dropEvent(QDropEvent *event) {
   // qDebug("singleWidget drop");

    if(handleDragMoves(event)) {
        iColourGroup.handleMimeData(event->mimeData()->data(mimeType()), *this, false);

        update();
    }

   /*  if (event->mimeData()->hasFormat(mimeType())) {


         if (event->source() != 0 && event->source() != this) {
             event->accept();
         }
     } else {
         event->ignore();
     }*/
}
