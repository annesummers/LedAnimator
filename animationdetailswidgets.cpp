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


  //  QScrollArea* scrollArea = new QScrollArea;
   // scrollArea->setBackgroundRole(QPalette::Dark);
   // scrollArea->setWidget(this);

   // scrollArea->setAcceptDrops(true);
    setAcceptDrops(true);

  //  scrollArea->setMaximumSize(parent->parentWidget()->width(), parent->parentWidget()->height());
    //setMinimumSize(parent->parentWidget()->width(), parent->parentWidget()->height());
    //iLayout = new QVBoxLayout(this);

    iFrameSlider = new QSlider(Qt::Horizontal, this);
    iFrameSlider->setMinimum(INITIAL_FRAME);  // frames are indexed from 1
    iFrameSlider->resize(parent->width() - BORDER*2, SLIDER_HEIGHT);
    iFrameSlider->setTickPosition(QSlider::TicksBelow);
   // iLayout->addWidget(iFrameSlider, 0, Qt::AlignTop);

    iLedDetailsList = new LedDetailsListWidget(this, animation);
    iLedDetailsList->move(iLedDetailsList->pos().x(), iLedDetailsList->pos().y() + SLIDER_HEIGHT);
    iLedDetailsList->resize(parent->width() - BORDER*2, height() - SLIDER_HEIGHT - 5);

    //iLayout->addWidget(iLedDetailsList);

   // setLayout(iLayout);


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
    iFrameSlider->setTickInterval(numFrames);
}

// TODO this is bollocks
void AnimationDetailsWidget::frameListPosition(int x, int width) {
    iFramesListX = x;
    iFramesListWidth = width;
    int frameIncrements = iFramesListWidth/(iAnimation.numFrames());

    iFrameSlider->move(x + frameIncrements/2, iFrameSlider->y());
    iFrameSlider->resize(width-frameIncrements+frameIncrements/4, iFrameSlider->height());
}

// events ---------

void AnimationDetailsWidget::paintEvent(QPaintEvent *) {
    if(iFramesListWidth > 0) {
        int currentFrameIncrements = iFramesListWidth/(iAnimation.numFrames());
        int currentFramePosition = (iAnimation.currentFrame() - 1)*currentFrameIncrements;

        QPainter painter(this);
        painter.setPen(Qt::black);
        painter.drawLine(QPoint(currentFramePosition + (iFramesListX + currentFrameIncrements/2), iFrameSlider->height() - 8), QPoint(currentFramePosition + iFramesListX + currentFrameIncrements/2 + 2, height() - iFrameSlider->height() - 8));
    }
}

void AnimationDetailsWidget::resizeEvent(QResizeEvent*) {
    resize(parentWidget()->width() - BORDER*2, parentWidget()->height());

    // slider must start from the start of the frames
    iFrameSlider->resize(width(), SLIDER_HEIGHT);
    iLedDetailsList->resize(width(), height() - SLIDER_HEIGHT);
}

void AnimationDetailsWidget::dragEnterEvent(QDragEnterEvent* event) {
    if (event->mimeData()->hasFormat(LED_MIME_TYPE)) {
         if (event->source() != 0) {
             event->setDropAction(Qt::LinkAction);
             event->accept();
         }
     } else {
         event->ignore();
     }
}

void AnimationDetailsWidget::dragMoveEvent(QDragMoveEvent* event) {
    if (event->mimeData()->hasFormat(LED_MIME_TYPE)) {
        if (event->source() != 0) {
            event->setDropAction(Qt::LinkAction);
            event->accept();
        }
    } else {
        event->ignore();
    }
}

void AnimationDetailsWidget::dropEvent(QDropEvent *event) {
     if (event->mimeData()->hasFormat(LED_MIME_TYPE)) {
         QByteArray itemData = event->mimeData()->data(LED_MIME_TYPE);
         QDataStream dataStream(&itemData, QIODevice::ReadOnly);

         if (event->source() != 0) {
             event->setDropAction(Qt::LinkAction);
             event->accept();
         }

         QColor colour;
         int row;
         int column;

         int numRows, numColumns;
         dataStream >> numRows >> numColumns;

         for(int i = 0; i < numRows * numColumns; i++) {
            dataStream >> colour >> row >> column;
            iLedDetailsList->addLed(row, column);
         }

         update();
     } else {
         event->ignore();
     }
 }



