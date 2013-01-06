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
    iItem(item) {

    setAcceptDrops(true);

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

const QByteArray ColourWidget::mimeData() {
    if(iColourGroup.isGroupSelected()) {
        return iColourGroup.mimeData();
    }

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);

    dataStream << 1 << colour();

    addExtraData(dataStream);

    return itemData;
}

void ColourWidget::handleMimeData(QByteArray itemData) {
    if(iColourGroup.isGroupSelected()) {
        iColourGroup.handleMimeData(itemData, *this);
    }

    QDataStream dataStream(&itemData, QIODevice::ReadOnly);

    int num;
    QColor colour;

    dataStream >> num >> colour;

    setColour(colour);
    handleExtraData(dataStream);
}

void ColourWidget::copy() {
    QClipboard *clipboard = QApplication::clipboard();
    QMimeData *data = new QMimeData;

    data->setData(mimeType(), mimeData());
    clipboard->setMimeData(data);
}

void ColourWidget::paste() {
    const QClipboard *clipboard = QApplication::clipboard();

    if(clipboard->mimeData()->hasFormat(mimeType())) {
        handleMimeData(clipboard->mimeData()->data(mimeType()));
    }
}

// events ----------------------------------------

void ColourWidget::mousePressEvent(QMouseEvent* event) {
    qDebug("singleWidget mousePress");
    if (event->button() != Qt::LeftButton) {
        return;
    }

    iDragStartPosition = event->pos();

    if((QApplication::keyboardModifiers() & Qt::ControlModifier) != 0) {
         iColourGroup.select(*this, !iItem.isSelected());
         return;
    }

    if((QApplication::keyboardModifiers() & Qt::ShiftModifier) != 0) {
        iColourGroup.selectArea(*this);
        return;
    }
}

void ColourWidget::mouseReleaseEvent(QMouseEvent* event){
    if (event->button() != Qt::LeftButton) {
        return;
    }

    if((QApplication::keyboardModifiers() & Qt::ControlModifier) != 0 ||
       (QApplication::keyboardModifiers() & Qt::ShiftModifier) != 0) {
         return;
    }

    iColourGroup.selectOne(*this);
}

void ColourWidget::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() != Qt::LeftButton) {
        return;
    }

    if ((event->pos() - iDragStartPosition).manhattanLength()
             < QApplication::startDragDistance()) {
       return;
    }

    if(iColourGroup.isGroupSelected() && !iItem.isSelected()) {
        iColourGroup.selectOne(*this);
    }

    QMimeData *data = new QMimeData;

    data->setData(mimeType(), mimeData());

    QDrag *drag = new QDrag(this);
    drag->setMimeData(data);
    drag->setHotSpot(pos());

    if (drag->exec(Qt::CopyAction | dropAction()) == Qt::CopyAction | dropAction()) {
     //qDebug(QString("Successfully dragged and dropped led %1,%2").arg(iLed.row()).arg(iLed.column()));
    } else {
     //qDebug(QString("Wrong! led %1,%2").arg(iLed.row()).arg(iLed.column()));
    }
}

void ColourWidget::mouseDoubleClickEvent(QMouseEvent* event) {
    qDebug("singleWidget mouseDoubleClick");
    if (event->buttons() != Qt::LeftButton) {
        return;
    }

    iColourGroup.select(*this, true);

    QColor colour = QColorDialog::getColor(Qt::white,
                                           this,
                                           "Select Color",
                                           QColorDialog::DontUseNativeDialog);
    if(colour.isValid()) {
        iColourGroup.setColour(colour);
    }
}

void ColourWidget::contextMenuEvent(QContextMenuEvent *event) {
    QMenu menu(this);

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

         handleMimeData(event->mimeData()->data(mimeType()));

         if (event->source() != 0 && event->source() != this) {
             event->setDropAction(Qt::CopyAction);
             event->accept();
         }

         update();
     } else {
         event->ignore();
     }
}
