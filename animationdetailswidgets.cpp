#include "animationdetailswidgets.h"

#include "mainwindow.h"
#include "leddetailswidgets.h"
#include "animation.h"

#include "defaults.h"

AnimationDetailsWidget::AnimationDetailsWidget(QWidget* parent, Animation &animation) :
    QWidget(parent),
    iAnimation(animation),
    iFrameSlider(NULL),
    added(5){

    setAcceptDrops(true);

    iFrameSlider = new QSlider(Qt::Horizontal, this);
    iFrameSlider->setMinimum(1);  // frames are indexed from 1

    iLedDetailsList = new LedDetailsListWidget(this, animation);
    iLedDetailsList->move(iLedDetailsList->pos().x(), iLedDetailsList->pos().y() + 40);

    QObject::connect(iFrameSlider, SIGNAL(valueChanged(int)), &(animation), SLOT(setCurrentFrame(int)));
}

// slots -----------

void AnimationDetailsWidget::currentFrameChanged(int currentFrame) {
    iFrameSlider->setValue(currentFrame);
}

void AnimationDetailsWidget::numFramesChanged(int numFrames) {
    iFrameSlider->setMaximum(numFrames);
}

void AnimationDetailsWidget::newLed(Led& led) {
    if(added > 0){
        iLedDetailsList->addLed(led);
        added--;
    }
   // iFrameSlider->move(iLedDetailsList->frameListStartX()+5, iFrameSlider->pos().y());
}

// ----------------

// events ---------

void AnimationDetailsWidget::resizeEvent(QResizeEvent*) {
    resize(parentWidget()->width(), parentWidget()->height());

    // slider must start from the start of the frames
    iFrameSlider->resize(width() - LED_GRID_BORDER*2, iFrameSlider->height());
    iLedDetailsList->resize(width() - LED_GRID_BORDER*2, height());
}

void AnimationDetailsWidget::dragEnterEvent(QDragEnterEvent* event) {
  /*  if (event->mimeData()->hasFormat("application/x-leditemdata")) {
         if (event->source() != 0) {
             event->setDropAction(Qt::LinkAction);
             event->accept();
         }
     } else {*/
         event->ignore();
   //  }
}

void AnimationDetailsWidget::dragMoveEvent(QDragMoveEvent* event) {
   /* if (event->mimeData()->hasFormat("application/x-leditemdata")) {
        if (event->source() != 0) {
            event->setDropAction(Qt::LinkAction);
            event->accept();
        }
    } else {*/
        event->ignore();
  //  }
}

void AnimationDetailsWidget::dropEvent(QDropEvent *event) {
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



