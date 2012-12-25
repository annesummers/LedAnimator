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

#include <QPainter>

using namespace Ui;

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
        painter.setPen(Qt::DashLine);
    } else {
        painter.setPen(Qt::NoPen);
    }

    QRect rect(1, 1, width() - 1, height() - 1);

    painter.drawRect(rect);
}

void FrameWidget::mouseDoubleClickEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {

        QColor colour = QColorDialog::getColor(Qt::green, this, "Select Color", QColorDialog::DontUseNativeDialog);
        if(colour.isValid()) {
            Selectable* item = NULL;

            foreach(item, iSelectableGroup.selectedItems()) {
                static_cast<Frame*>(item)->setColour(colour);
            }
        }
    }
}

// ---------------------------

FrameListWidget::FrameListWidget(QWidget *parent, const Animation& animation, const Led &led) :
    SelectableGroupWidget(parent),
    iLed(led) {

    iFramesList = new QHBoxLayout(this);
    setLayout(iFramesList);

    numFramesChanged(animation.numFrames());

    connect(&(animation), SIGNAL(numFramesChanged(int)), this, SLOT(numFramesChanged(int)));
}

void FrameListWidget::numFramesChanged(int numFrames) {
      int w = width();
    int extra = width()%numFrames;
    int frameWidth = (width()-extra)/numFrames;

    int oldNumFrames = iFramesList->count();
    if(numFrames > oldNumFrames) {  // we need to add more frames
        for(int i = oldNumFrames; i < numFrames; i++) {
            FrameWidget* frame = new FrameWidget(this, *this, iLed.frameAt(i + INITIAL_FRAME));
            iFramesList->addWidget(frame);

            frame->resize(frameWidth, height());
            frame->setMinimumWidth(frameWidth);
            //frame->setMaximumWidth(frameWidth);
            frame->setMinimumHeight(height());
           // frame->setMaximumHeight(height());
        }
    }

    setMinimumWidth(frameWidth * iFramesList->count() + 2);
    //setMaximumWidth(frameWidth * iFramesList->count() + 2);
    setMinimumHeight(height());
    //setMaximumHeight(height());

    emit resized(pos().x(), width());
}

void FrameListWidget::resizeEvent(QResizeEvent *) {
   /* int numFrames = iFramesList->count();
    if(numFrames > 0) {
        int w = width();
        int extra = width()%numFrames;
        int frameWidth = (width()-extra)/numFrames;

        int oldNumFrames = iFramesList->count();
        if(numFrames > oldNumFrames) {  // we need to add more frames
            for(int i = oldNumFrames; i < numFrames; i++) {
                FrameWidget* frame = new FrameWidget(this, *this, iLed.frameAt(i + INITIAL_FRAME));
                iFramesList->addWidget(frame);

                frame->resize(frameWidth, height());
                frame->setMinimumWidth(frameWidth);
                //frame->setMaximumWidth(frameWidth);
                frame->setMinimumHeight(height());
                //frame->setMaximumHeight(height());
            }
        }

        setMinimumWidth(frameWidth * iFramesList->count() + 2);
      //  setMaximumWidth(frameWidth * iFramesList->count() + 2);
        setMinimumHeight(height());
       // setMaximumHeight(height());
}*/
    int numFrames = iFramesList->count();

    if(numFrames > 0) {
        int extra = width()%numFrames;
        int frameWidth = (width()-extra)/numFrames;

        for(int i = 0; i < numFrames; i++) {
            iFramesList->itemAt(i)->widget()->resize(frameWidth, height()-4);
        }
    }
}

void FrameListWidget::paintEvent(QPaintEvent *){
   /* QPainter painter(this);
    painter.setBrush(Qt::black);
    painter.drawRect(0, 0, width(), height());*/
}

void FrameListWidget::dragEnterEvent(QDragEnterEvent* event) {
  /*  if (event->mimeData()->hasFormat("application/x-leditemdata")) {
         if (event->source() != 0) {
             event->setDropAction(Qt::LinkAction);
             event->accept();
         }
     } else {*/
         event->ignore();
   //  }
}

void FrameListWidget::dragMoveEvent(QDragMoveEvent* event) {
   /* if (event->mimeData()->hasFormat("application/x-leditemdata")) {
        if (event->source() != 0) {
            event->setDropAction(Qt::LinkAction);
            event->accept();
        }
    } else {*/
        event->ignore();
  //  }
}

void FrameListWidget::dropEvent(QDropEvent *event) {
     if (event->mimeData()->hasFormat("application/x-leditemdata")) {
         /*QByteArray itemData = event->mimeData()->data("application/x-leditemdata");
         QDataStream dataStream(&itemData, QIODevice::ReadOnly);

         int row;
         int column;
         dataStream >> row >> column;

         Led& led = iAnimation.ledAt(row, column);

         if (event->source() != 0) {
             event->setDropAction(Qt::LinkAction);
             event->accept();
         }

         iLedDetailsList->addLed(led);

         update();*/
     } else {
         event->ignore();
     }
 }

