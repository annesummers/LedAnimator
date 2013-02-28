#include "framelistwidget.h"

#include "animation.h"
#include "led.h"
#include "framewidget.h"
#include "selectablegroupgroupwidget.h"

#include "exceptions.h"

#include "fadecalculator.h"

using namespace Exception;
using namespace AnimatorUi;

FrameListWidget::FrameListWidget(QWidget *parent,
                                 const Animation& animation,
                                 const Led &led,
                                 SelectableGroupGroupWidget &framesListGroup) :
    ColourGroupWidget(parent, 0, animation.numFrames(), &framesListGroup),
    iLed(led) {

    numFramesChanged(animation.numFrames());

    setFocusPolicy(Qt::ClickFocus);

    connect(&(animation), SIGNAL(numFramesChanged(int)), this, SLOT(numFramesChanged(int)));
}

// slots --------------------

void FrameListWidget::numFramesChanged(int numFrames) {
    QElapsedTimer timer;
    timer.start();
    qDebug("numFramesChanged");


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
       // update();

        // TODO why doesn't this work?
    }

    qDebug("0 %d", timer.elapsed());
}

// from SelectableWidget -----------------------------

SelectableWidget &FrameListWidget::widgetAt(int row, int column) {
    ColourGroupWidget::widgetAt(row, column);

    if(row != 0) {
        throw IllegalArgumentException("FrameListWidget::widgetAt : Row should be zero");
    }

    return *iFramesList.at(column);
}

void FrameListWidget::getWidgetPosition(SelectableWidget& widget, int* row, int* column) {
    *column = iFramesList.indexOf(&static_cast<FrameWidget&>(widget));
    *row = 0;

    ColourGroupWidget::getWidgetPosition(widget, row, column);
}

bool FrameListWidget::validKeyPress(Qt::Key key) {
    return (key & Qt::Key_Right) == Qt::Key_Right ||
           (key & Qt::Key_Left) == Qt::Key_Left;
}

void FrameListWidget::copyItem(int fromGroup, int fromRow, int fromColumn, int toRow, int toColumn) {
    FrameWidget* fromWidget;

    if(fromGroup == iGroupNumber) {
        fromWidget = static_cast<FrameWidget*>(&widgetAt(fromRow, fromColumn));
    } else {
        FrameListWidget& group = static_cast<FrameListWidget&>(iGroupGroup->group(fromGroup));
        fromWidget = static_cast<FrameWidget*>(&group.widgetAt(fromRow, fromColumn));
    }

    FrameWidget& toWidget = static_cast<FrameWidget&>(widgetAt(toRow, toColumn));

    toWidget.frame().setColour(fromWidget->frame().colour());
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
