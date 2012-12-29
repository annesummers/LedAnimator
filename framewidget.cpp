/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include "framewidget.h"

#include "frame.h"
#include "led.h"
#include "animation.h"
#include "selectablegroupwidget.h"
#include "selectablewidget.h"

#include "exceptions.h"

#include <QPainter>

using namespace Ui;
using namespace Exception;

FrameWidget::FrameWidget(QWidget* parent, SelectableGroupWidget& frameGroup, Frame& frame) :
    DragDropWidget(parent, frameGroup, frame) {

    copyAct = new QAction(tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy this colour"));
    connect(copyAct, SIGNAL(triggered()), this, SLOT(copy()));

    pasteAct = new QAction(tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the copied colour"));
    connect(pasteAct, SIGNAL(triggered()), this, SLOT(paste()));

    connect(&frame, SIGNAL(colourChanged()), this, SLOT(frameColourChanged()));
}

void FrameWidget::frameColourChanged() {
    setToolTip(QString("h: %1\ns: %2\nv: %3")
               .arg(frame().colour().hue())
               .arg(frame().colour().saturation())
               .arg(frame().colour().value()));
    update();
}

void FrameWidget::copy() {
    QClipboard *clipboard = QApplication::clipboard();
    QMimeData *mimeData = new QMimeData;
    mimeData->setData(mimeType(), dragData());

    clipboard->setMimeData(mimeData);
}

void FrameWidget::paste() {
    const QClipboard *clipboard = QApplication::clipboard();
    handleDragData(clipboard->mimeData()->data(mimeType()));
}

// from DraggableWidget ----------------------------

const QByteArray FrameWidget::dragData() const {
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);

    dataStream << frame().colour();

    return itemData;
}

void FrameWidget::handleDragData(QByteArray itemData) {
    QDataStream dataStream(&itemData, QIODevice::ReadOnly);
    QColor colour;

    dataStream >> colour;

    frame().setColour(colour);
}

// events -----------------------------

void FrameWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setBrush(frame().colour());

    if(frame().isSelected() ){
        painter.setPen(Qt::white);
    } else {
        painter.setPen(Qt::black);
    }

    QRect rect(0, 0, width(), height());

    painter.drawRect(rect);

    painter.setBrush(Qt::NoBrush);
    painter.drawRect(0, 0, width() - 1, height() - 1);
}

void FrameWidget::mouseDoubleClickEvent(QMouseEvent* event) {
    if (event->button() != Qt::LeftButton) {
        return;
    }

    iSelectableGroup.select(*this, true);

    QColor colour = QColorDialog::getColor(Qt::green, this, "Select Color", QColorDialog::DontUseNativeDialog);
    if(colour.isValid()) {
        SelectableWidget* item = NULL;

        for(int i = 0; i < iSelectableGroup.selectedItems().count(); i++) {
            (static_cast<Frame&>(iSelectableGroup.selectedItems().at(i)->item())).setColour(colour);
        }

        foreach(item, iSelectableGroup.selectedItems()) {
            static_cast<Frame&>(item->item()).setColour(colour);
        }
    }
}

void FrameWidget::contextMenuEvent(QContextMenuEvent *event) {
    QMenu menu(this);
    menu.addAction(copyAct);
    menu.addAction(pasteAct);
    menu.exec(event->globalPos());
}

