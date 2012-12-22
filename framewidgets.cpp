/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include "framewidgets.h"

#include "frame.h"
#include "led.h"
#include "animation.h"

#include <QPainter>

using namespace Ui;

Ui::FrameWidget::FrameWidget(Frame& frame) :
    iFrame(frame) {

    QObject::connect(&frame, SIGNAL(colourChanged()), this, SLOT(frameColourChanged()));
   // QObject::connect(&frame, SIGNAL(selected()), this, SLOT(frameSelected()));
}

void Ui::FrameWidget::frameColourChanged() {
    setToolTip(QString("h: %1\ns: %2\nv: %3")
               .arg(iFrame.colour().hue())
               .arg(iFrame.colour().saturation())
               .arg(iFrame.colour().value()));
    update();
}

void Ui::FrameWidget::frameSelected() {
    update();
}

void Ui::FrameWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setBrush(iFrame.colour());
    painter.setPen(Qt::NoPen);

    QRect rect(1, 0, width() - 2, height() - 1);

    painter.drawRect(rect);
}

void Ui::FrameWidget::mouseDoubleClickEvent(QMouseEvent* event) {
    Q_UNUSED(event);
    // TODO change colour
}

void Ui::FrameWidget::mousePressEvent(QMouseEvent* event) {
    Q_UNUSED(event);
    //TODO drag and drop
}

// ---------------------------

Ui::FrameListWidget::FrameListWidget(QWidget *parent, const Animation& animation, const Led &led) :
    QWidget(parent),
    iLed(led) {

    iFramesList = new QHBoxLayout(this);
    setLayout(iFramesList);

    QObject::connect(&(animation), SIGNAL(numFramesChanged(int)), this, SLOT(numFramesChanged(int)));
}

void Ui::FrameListWidget::numFramesChanged(int numFrames) {

    int extra = parentWidget()->width()%numFrames;
    int frameWidth = (parentWidget()->width()-extra)/numFrames;

    int oldNumFrames = iFramesList->count();
    if(numFrames > oldNumFrames) {  // we need to add more frames
        for(int i = oldNumFrames; i < numFrames; i++) {
            FrameWidget* frame = new FrameWidget(iLed.frameAt(i + INITIAL_FRAME));
            iFramesList->addWidget(frame);

            frame->resize(frameWidth, height());
            frame->setMinimumWidth(frameWidth);
            //frame->setMaximumWidth(frameWidth);
            frame->setMinimumHeight(height());
            //frame->setMaximumHeight(height());
        }
    } else {  // we have to remove frames
        for(int i = oldNumFrames; i > numFrames; i--) {
            QLayoutItem* item = iFramesList->itemAt(i);
            iFramesList->removeItem(item);
        }
    }

    setMinimumWidth(frameWidth * iFramesList->count() + 2);
  //  setMaximumWidth(frameWidth * iFramesList->count() + 2);
    setMinimumHeight(height());
   // setMaximumHeight(height());

    emit resized(pos().x(), width());
}

void Ui::FrameListWidget::resizeEvent(QResizeEvent *) {
    int numFrames = iFramesList->count();

    if(numFrames > 0) {
        int extra = parentWidget()->width()%numFrames;
        int frameWidth = (parentWidget()->width()-extra)/numFrames;

        for(int i = 0; i < numFrames; i++) {
            iFramesList->itemAt(i)->widget()->resize(frameWidth, height() - 8);
        }
    }
}
