/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include "framewidgets.h"

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
    SelectableWidget(parent, frameGroup, frame) {

    connect(&frame, SIGNAL(colourChanged()), this, SLOT(frameColourChanged()));
}

void FrameWidget::frameColourChanged() {
    setToolTip(QString("h: %1\ns: %2\nv: %3")
               .arg(frame().colour().hue())
               .arg(frame().colour().saturation())
               .arg(frame().colour().value()));
    update();
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

// ---------------------------

FrameListWidget::FrameListWidget(QWidget *parent, const Animation& animation, const Led &led) :
    SelectableGroupWidget(parent, 0, animation.numFrames()),
    iLed(led) {

    //iFramesList = new QHBoxLayout(this);
    //setLayout(iFramesList);

    numFramesChanged(animation.numFrames());

    setFocusPolicy(Qt::ClickFocus);

    connect(&(animation), SIGNAL(numFramesChanged(int)), this, SLOT(numFramesChanged(int)));
}

void FrameListWidget::numFramesChanged(int numFrames) {
    // TODO what if threre are fewer frames?

   /*   int w = width();
    int extra = width()%numFrames;
    int frameWidth = (width()-extra)/numFrames;

    resize(width()-extra, height());*/

    int oldNumFrames = iFramesList.count();
    if(numFrames > oldNumFrames) {  // we need to add more frames
        for(int i = oldNumFrames; i < numFrames; i++) {
            FrameWidget* frame = new FrameWidget(this, *this, iLed.frameAt(i + INITIAL_FRAME));
            iFramesList.append(frame);

           // frame->resize(frameWidth, height());

          //  frame->setMaximumWidth(frameWidth+2);
          //  frame->setMaximumHeight(height()+2);
        }
    }

    resize(parentWidget()->width(), height());

    //setMinimumWidth((frameWidth) * iFramesList->count());
   // setMinimumHeight(height() + 2);
   // resize((frameWidth) * iFramesList->count(), height());

    emit resized(pos().x(), width());
}

SelectableWidget& FrameListWidget::widgetAt(int row, int column) {
    if(row != 0) {
        throw IllegalArgumentException("Row should be zero");
    }

    return *iFramesList.at(column);
}

void FrameListWidget::getWidgetPosition(SelectableWidget& widget, int* row, int* column) {
    *column = iFramesList.indexOf(&static_cast<FrameWidget&>(widget));
    *row = 0;
}

void FrameListWidget::keyPressEvent(QKeyEvent *event) {
    if((QApplication::keyboardModifiers() & Qt::ShiftModifier) != 0) {

        QList<SelectableWidget*> selectedWidgets = selectedItems();

        if(selectedWidgets.count() > 0) {
            if((event->key() & Qt::RightArrow) != 0 ||
               (event->key() & Qt::LeftArrow) != 0) {

                FrameWidget& last = static_cast<FrameWidget&>(*selectedWidgets.at(selectedWidgets.count() - 1));
                int column = iFramesList.indexOf(&last);

                if(event->key() == Qt::Key_Right) {
                    selectRight(column);
                } else if(event->key() == Qt::Key_Left) {
                    selectLeft(column);
                }
            }
        }
    }
}

void FrameListWidget::resizeEvent(QResizeEvent *) {
    int numFrames = iFramesList.count();

    if(numFrames > 0) {
        int extra = width()%numFrames;
        int frameWidth = (width()-extra)/numFrames;

        for(int i = 0; i < numFrames; i++) {
            iFramesList.at(i)->resize(frameWidth, height());
            iFramesList.at(i)->move(frameWidth*i, 0);
        }

        resize(width()-extra, height());
    }

    emit resized(x() + 10, width() + 7);
}

void FrameListWidget::paintEvent(QPaintEvent *){
    //QPainter painter(this);
    //painter.setBrush(Qt::black);
    //painter.drawRect(0, 0, width(), height());
}

void FrameListWidget::dragEnterEvent(QDragEnterEvent* event) {
    if (event->mimeData()->hasFormat("application/x-frameitemdata")) {
         if (event->source() != 0) {
             event->accept();
         }
     } else {
         event->ignore();
     }
}

void FrameListWidget::dragMoveEvent(QDragMoveEvent* event) {
    if (event->mimeData()->hasFormat("application/x-frameitemdata")) {
        if (event->source() != 0) {
            event->accept();
        }
    } else {
        event->ignore();
    }
}

void FrameListWidget::dropEvent(QDropEvent *event) {
     if (event->mimeData()->hasFormat("application/x-frameitemdata")) {
         QByteArray itemData = event->mimeData()->data("application/x-leditemdata");
         QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        if (event->source() != 0) {
             event->setDropAction(Qt::CopyAction);
             event->accept();
         }
     } else {
         event->ignore();
     }
 }

