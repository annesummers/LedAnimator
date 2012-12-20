#include "framewidgets.h"

#include "frame.h"
#include "led.h"
#include "animation.h"

#include <QPainter>

FrameWidget::FrameWidget(QWidget *parent, Frame& frame) :
    QWidget(parent),
    iFrame(frame) {

    QObject::connect(&frame, SIGNAL(colourChanged()), this, SLOT(frameColourChanged()));
   // QObject::connect(&frame, SIGNAL(selected()), this, SLOT(frameSelected()));
}

void FrameWidget::frameColourChanged() {
    setToolTip(QString("h: %1\ns: %2\nv: %3")
               .arg(iFrame.colour().hue())
               .arg(iFrame.colour().saturation())
               .arg(iFrame.colour().value()));
    update();
}

void FrameWidget::frameSelected() {
    update();
}

void FrameWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setBrush(iFrame.colour());
    painter.setPen(Qt::NoPen);

    QRect rect(0, 0, width() - 1, height() - 1);

    painter.drawRect(rect);
}

void FrameWidget::mouseDoubleClickEvent(QMouseEvent* event) {

    // TODO change colour
}

void FrameWidget::mousePressEvent(QMouseEvent* event) {

    //TODO drag and drop
}

// ---------------------------

FrameListWidget::FrameListWidget(QWidget *parent, Animation& animation, Led &led) :
    QWidget(parent),
    iLed(led) {

    iFramesList = new QHBoxLayout(this);
    setLayout(iFramesList);

    QObject::connect(&(animation), SIGNAL(numFramesChanged(int)), this, SLOT(numFramesChanged(int)));
}

void FrameListWidget::numFramesChanged(int numFrames) {

    int frameWidth = parentWidget()->width()/numFrames;
    int oldNumFrames = iFramesList->count();
    if(numFrames > oldNumFrames) {  // we need to add more frames
        for(int i = oldNumFrames; i < numFrames; i++) {
            FrameWidget* frame = new FrameWidget(this, iLed.frameAt(i));
            iFramesList->addWidget(frame);

            frame->resize(frameWidth, height() - 8);
        }
    } else {  // we have to remove frames
        for(int i = oldNumFrames; i > numFrames; i--) {
            QLayoutItem* item = iFramesList->itemAt(i);
            iFramesList->removeItem(item);
        }
    }

    resize(numFrames*frameWidth, height());
}

void FrameListWidget::resizeEvent(QResizeEvent *) {
    int numFrames = iFramesList->count();

    if(numFrames > 0) {
        int extra = parentWidget()->width()%numFrames;
        int frameWidth = (parentWidget()->width()-extra)/numFrames;

        for(int i = 0; i < numFrames; i++) {
            iFramesList->itemAt(i)->widget()->resize(frameWidth, height() - 8);
        }
    }
}
