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
    iFramesListX(0),
    iFramesListWidth(0),
    iFrameSlider(NULL),
    added(5){

    setAcceptDrops(true);

    iFrameSlider = new QSlider(Qt::Horizontal, this);
    iFrameSlider->setMinimum(INITIAL_FRAME);  // frames are indexed from 1
    iFrameSlider->resize(width() - BORDER*2, SLIDER_HEIGHT);

    iLedDetailsList = new LedDetailsListWidget(this, animation);
    iLedDetailsList->move(iLedDetailsList->pos().x(), iLedDetailsList->pos().y() + SLIDER_HEIGHT);
    iLedDetailsList->resize(width() - BORDER*2, height() - SLIDER_HEIGHT);

    connect(iFrameSlider, SIGNAL(valueChanged(int)), &animation, SLOT(setCurrentFrame(int)));

    connect(&animation, SIGNAL(numFramesChanged(int)), this, SLOT(numFramesChanged(int)));
    connect(&animation, SIGNAL(currentFrameChanged(int)), this, SLOT(currentFrameChanged(int)));
}

// slots -----------

void AnimationDetailsWidget::currentFrameChanged(int currentFrame) {
    iFrameSlider->setValue(currentFrame);
    update();
}

void AnimationDetailsWidget::numFramesChanged(int numFrames) {
    iFrameSlider->setMaximum(numFrames);
}

// TODO this is bollocks
void AnimationDetailsWidget::frameListPosition(int x, int width) {
    iFrameSlider->move(x-2, iFrameSlider->y());
    iFrameSlider->resize(width+2, iFrameSlider->height());
    iFramesListX = x;
    iFramesListWidth = width;
}

// ----------------

// events ---------

void AnimationDetailsWidget::paintEvent(QPaintEvent *) {
    if(iFramesListWidth > 0) {
        int currentFrameIncrements = iFramesListWidth/(iAnimation.numFrames());
        int currentFramePosition = (iAnimation.currentFrame() - 1)*currentFrameIncrements;

        QPainter painter(this);
        painter.setPen(Qt::black);
        painter.drawLine(QPoint(currentFramePosition + (iFramesListX + currentFrameIncrements/2) + 2, iFrameSlider->height() - 8), QPoint(currentFramePosition + iFramesListX + currentFrameIncrements/2 + 2, height() - iFrameSlider->height() - 8));
    }
}

void AnimationDetailsWidget::resizeEvent(QResizeEvent*) {
    resize(parentWidget()->width() - BORDER*2, parentWidget()->height());

    // slider must start from the start of the frames
    iFrameSlider->resize(width(), SLIDER_HEIGHT);
    iLedDetailsList->resize(width(), height() - SLIDER_HEIGHT);
}

void AnimationDetailsWidget::dragEnterEvent(QDragEnterEvent* event) {
    if (event->mimeData()->hasFormat("application/x-leditemdata")) {
         if (event->source() != 0) {
             event->accept();
         }
     } else {
         event->ignore();
     }
}

void AnimationDetailsWidget::dragMoveEvent(QDragMoveEvent* event) {
    if (event->mimeData()->hasFormat("application/x-leditemdata")) {
        if (event->source() != 0) {
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



