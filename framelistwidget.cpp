#include "framelistwidget.h"

#include "animation.h"
#include "led.h"
#include "framewidget.h"
#include "colourgroupgroupwidget.h"

#include "exceptions.h"

#include "fadecalculator.h"

using namespace Exception;
using namespace Ui;

FrameListWidget::FrameListWidget(QWidget *parent,
                                 const Animation& animation,
                                 const Led &led,
                                 ColourGroupGroupWidget &framesListGroup) :
    ColourGroupWidget(parent, 0, animation.numFrames(), &framesListGroup),
    iLed(led) {

    numFramesChanged(animation.numFrames());

    setFocusPolicy(Qt::ClickFocus);

    connect(&(animation), SIGNAL(numFramesChanged(int)), this, SLOT(numFramesChanged(int)));
}

// slots --------------------

void FrameListWidget::numFramesChanged(int numFrames) {

    int oldNumFrames = iFramesList.count();

    if(numFrames > oldNumFrames) {  // we need to add more frames
        for(int i = oldNumFrames; i < numFrames; i++) {
            FrameWidget* frame = new FrameWidget(this, *this, iLed.frameAt(i + INITIAL_FRAME));
            iFramesList.append(frame);
        }
    } else if (numFrames < oldNumFrames ) {  // we need to remove some frames; take them from the end
        for(int i = oldNumFrames; i >= numFrames; i--) {
            FrameWidget* frame = iFramesList.at(i);

            iFramesList.removeAt(i);
            delete frame;
        }
    }

    if(oldNumFrames != numFrames) {
       // resize(parentWidget()->width(), height());

       // emit resized(pos().x(), width());
    }
}

// from ColourGroupWidget -----------------------------

ColourWidget& FrameListWidget::widgetAt(int row, int column) {
    ColourGroupWidget::widgetAt(row, column);

    if(row != 0) {
        throw IllegalArgumentException("Row should be zero");
    }

    return *iFramesList.at(column);
}

void FrameListWidget::getWidgetPosition(ColourWidget& widget, int* row, int* column) {
    *column = iFramesList.indexOf(&static_cast<FrameWidget&>(widget));
    *row = 0;

    ColourGroupWidget::getWidgetPosition(widget, row, column);
}

bool FrameListWidget::validKeyPress(Qt::Key key) {
    return key & Qt::Key_Right != 0 ||
           key & Qt::Key_Left != 0;
}

// events ------------------------------------

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
