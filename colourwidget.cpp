/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include "colourwidget.h"

#include <QtGui>

#include "selectable.h"
#include "colourgroupwidget.h"

using namespace Ui;

ColourWidget::ColourWidget(QWidget* parent, ColourGroupWidget &groupWidget, Selectable& item) :
    QWidget(parent),
    iColourGroup(groupWidget),
    iItem(item),
    iDoubleClick(false),
    iColourDialog(NULL){

    setAcceptDrops(true);

    iSetColourAction = new QAction(tr("&Set colour"), this);
    iSetColourAction->setStatusTip(tr("Choose a colour"));

    connect(iSetColourAction, SIGNAL(triggered()), this, SLOT(chooseColour()));

    iCopyAction = new QAction(tr("&Copy"), this);
    iCopyAction->setShortcuts(QKeySequence::Copy);
    iCopyAction->setStatusTip(tr("Copy this colour"));

    connect(iCopyAction, SIGNAL(triggered()), this, SLOT(copy()));

    iPasteAction = new QAction(tr("&Paste"), this);
    iPasteAction->setShortcuts(QKeySequence::Paste);
    iPasteAction->setStatusTip(tr("Paste the copied colour"));

    connect(iPasteAction, SIGNAL(triggered()), this, SLOT(paste()));

    iFadeAction = new QAction(tr("&Fade"), this);
    iFadeAction->setStatusTip("Fade to colour");

    connect(iFadeAction, SIGNAL(triggered()), this, SLOT(fade()));

    connect(&item, SIGNAL(selected()), this, SLOT(selected()));

    iColourDialog = new QColorDialog(this);
    connect(iColourDialog, SIGNAL(currentColorChanged(QColor)), &iColourGroup, SLOT(setColour(QColor)));
}

void ColourWidget::chooseColour() {
    iColourDialog->exec();
}

void ColourWidget::colourChanged() {
    setToolTip(QString("h: %1\ns: %2\nv: %3")
               .arg(colour().hue())
               .arg(colour().saturation())
               .arg(colour().value()));
    update();
}

void ColourWidget::fade() {
    iColourGroup.fade();
}

// copy and paste ------------------------

void ColourWidget::copy() {
    QClipboard *clipboard = QApplication::clipboard();
    QMimeData *data = new QMimeData;

    data->setData(mimeType(), iColourGroup.mimeData());
    clipboard->setMimeData(data);
}

void ColourWidget::paste() {
    const QClipboard *clipboard = QApplication::clipboard();

    if(clipboard->mimeData()->hasFormat(mimeType())) {
        iColourGroup.handleMimeData(clipboard->mimeData()->data(mimeType()), *this);
    }
}

// events ----------------------------------------

void ColourWidget::mousePressEvent(QMouseEvent* event) {
    qDebug("singleWidget mousePress");
    if (event->button() != Qt::LeftButton) {
        return;
    }

    iDragStartPosition = event->pos();

    if((QApplication::keyboardModifiers() & Qt::ControlModifier) != 0 &&
         !iColourGroup.isGroupSelected()) {
         iColourGroup.select(*this, !iItem.isSelected());
         return;
    }

   /* if((QApplication::keyboardModifiers() & Qt::ShiftModifier) != 0) {
        iColourGroup.selectArea(*this);
        return;
    }*/
}

void ColourWidget::mouseReleaseEvent(QMouseEvent* event){
    qDebug("singleWidget mouseRelease");
    if (event->button() != Qt::LeftButton) {
        return;
    }

   // if((QApplication::keyboardModifiers() & Qt::ControlModifier) != 0 ||
   //    (QApplication::keyboardModifiers() & Qt::ShiftModifier) != 0) {
   //      return;
   // }

   /* if((QApplication::keyboardModifiers() & Qt::ControlModifier) != 0) {
             iColourGroup.select(*this, !iItem.isSelected());
             return;
        }*/

    if((QApplication::keyboardModifiers() & Qt::ShiftModifier) != 0) {
        iColourGroup.selectArea(*this);
        return;
    }

    if((QApplication::keyboardModifiers() & Qt::ControlModifier) != 0) {
        return;
    }

    if(!iDoubleClick) {
        iColourGroup.selectOne(*this);
    } else {
        iDoubleClick = false;
    }
}

void ColourWidget::mouseMoveEvent(QMouseEvent *event) {
    qDebug("mouse move event");
    if (event->buttons() != Qt::LeftButton) {
        qDebug("mouse move event NOT LEFT");
        return;
    }

    if ((event->pos() - iDragStartPosition).manhattanLength()
             < QApplication::startDragDistance()) {

        qDebug("mouse move event TOO SHORT");
       return;
    }

    if(!iItem.isSelected()) {
        iColourGroup.selectOne(*this);
    }

    QMimeData *data = new QMimeData;

    data->setData(mimeType(), iColourGroup.mimeData());

    QDrag *drag = new QDrag(this);
    drag->setMimeData(data);
    drag->setHotSpot(pos());

    if (drag->exec(Qt::CopyAction | dropAction()) == Qt::CopyAction | dropAction()) {
     qDebug("Successfully dragged and dropped item");
    } else {
     qDebug("Wrong!");
    }
}

void ColourWidget::mouseDoubleClickEvent(QMouseEvent* event) {
    qDebug("singleWidget mouseDoubleClick");
    iDoubleClick = true;
    if (event->buttons() != Qt::LeftButton) {
        return;
    }

    if(!iItem.isSelected()) {
        iColourGroup.selectOne(*this);
    }

    chooseColour();
}

void ColourWidget::contextMenuEvent(QContextMenuEvent *event) {
    if(!iColourGroup.isGroupSelected() && !iItem.isSelected()) {
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
    if (event->mimeData()->hasFormat(mimeType())) {
        if (event->source() != 0 && event->source() != this) {
            event->setDropAction(Qt::CopyAction);
            event->accept();
         }
     } else {
         event->ignore();
     }
}

void ColourWidget::dragMoveEvent(QDragMoveEvent* event) {
    if (event->mimeData()->hasFormat(mimeType())) {
        if (event->source() != 0 && event->source() != this) {
            event->setDropAction(Qt::CopyAction);
            event->accept();
        }
    } else {
        event->ignore();
    }
}

void ColourWidget::dropEvent(QDropEvent *event) {
     if (event->mimeData()->hasFormat(mimeType())) {

         iColourGroup.handleMimeData(event->mimeData()->data(mimeType()), *this);

         if (event->source() != 0 && event->source() != this) {
             event->setDropAction(Qt::CopyAction);
             event->accept();
         }

         update();
     } else {
         event->ignore();
     }
}
