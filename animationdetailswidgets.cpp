/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include "animationdetailswidgets.h"

#include "mainwindow.h"
#include "leddetailswidgets.h"
#include "animation.h"

#include "defaults.h"

using namespace Ui;

AnimationDetailsWidget::AnimationDetailsWidget(QWidget* parent, Animation &animation) :
    QWidget(parent),
    iAnimation(animation),
    iFrameSlider(NULL),
    added(5){

    setAcceptDrops(true);

    iFrameSlider = new QSlider(Qt::Horizontal, this);
    iFrameSlider->setMinimum(1);  // frames are indexed from 1
    iFrameSlider->resize(width(), SLIDER_HEIGHT);

    iLedDetailsList = new LedDetailsListWidget(this, animation);
    iLedDetailsList->move(iLedDetailsList->pos().x(), iLedDetailsList->pos().y() + SLIDER_HEIGHT);
    iLedDetailsList->resize(width(), height() - SLIDER_HEIGHT);

    connect(iFrameSlider, SIGNAL(valueChanged(int)), &animation, SLOT(setCurrentFrame(int)));

    connect(&animation, SIGNAL(numFramesChanged(int)), this, SLOT(numFramesChanged(int)));
    connect(&animation, SIGNAL(currentFrameChanged(int)), this, SLOT(currentFrameChanged(int)));
    ///connect(&animation, SIGNAL(newLed(int, int)), this, SLOT(newLed(int, int)));
}

// slots -----------

void AnimationDetailsWidget::currentFrameChanged(int currentFrame) {
    iFrameSlider->setValue(currentFrame);
}

void AnimationDetailsWidget::numFramesChanged(int numFrames) {
    iFrameSlider->setMaximum(numFrames);
}
/*
void AnimationDetailsWidget::newLed(int row, int column) {
   if(added > 0){
        iLedDetailsList->addLed(row, column);
        added--;
    }
}*/

// TODO this is bollocks
void AnimationDetailsWidget::frameListPosition(int x, int width) {
    iFrameSlider->move(x, iFrameSlider->y());
    iFrameSlider->resize(width, iFrameSlider->height());
}

// ----------------

// events ---------

void AnimationDetailsWidget::resizeEvent(QResizeEvent*) {
    resize(parentWidget()->width(), parentWidget()->height());

    // slider must start from the start of the frames
    iFrameSlider->resize(width() - BORDER*2, iFrameSlider->height());
    iLedDetailsList->resize(width() - BORDER*2, height());
}

void AnimationDetailsWidget::dragEnterEvent(QDragEnterEvent* event) {
    if (event->mimeData()->hasFormat("application/x-leditemdata")) {
         if (event->source() != 0) {
            // event->setDropAction(Qt::LinkAction);
             event->accept();
         }
     } else {
         event->ignore();
     }
}

void AnimationDetailsWidget::dragMoveEvent(QDragMoveEvent* event) {
    if (event->mimeData()->hasFormat("application/x-leditemdata")) {
        if (event->source() != 0) {
            //event->setDropAction(Qt::LinkAction);
            event->accept();
        }
    } else {
        event->ignore();
    }
}

void AnimationDetailsWidget::dropEvent(QDropEvent *event) {
     if (event->mimeData()->hasFormat("application/x-leditemdata")) {
         QByteArray itemData = event->mimeData()->data("application/x-leditemdata");
         QDataStream dataStream(&itemData, QIODevice::ReadOnly);

         int row;
         int column;
         dataStream >> row >> column;

         //Led& led = iAnimation.ledAt(row, column);

         if (event->source() != 0) {
             event->setDropAction(Qt::LinkAction);
             event->accept();
         }

         iLedDetailsList->addLed(row, column);

         update();
     } else {
         event->ignore();
     }
 }



