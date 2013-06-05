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
                                 ColourGroupGroupWidget &framesListGroup,
                                 int groupNumber) :
    ColourGroupWidget(parent, 0, animation.numFrames(), framesListGroup, groupNumber),
    iLed(led),
    iResized(false){

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
        doResize();
    }
}

// from SelectableWidget -----------------------------

SelectableWidget &FrameListWidget::widgetAt(Position position) {
    ColourGroupWidget::widgetAt(position);

    if(position.row() != 0) {
        throw IllegalArgumentException("FrameListWidget::widgetAt : Row should be zero");
    }

    return *iFramesList.at(position.column());
}

Position FrameListWidget::widgetPosition(SelectableWidget& widget) {
   return Position(0, iFramesList.indexOf(&static_cast<FrameWidget&>(widget)));
}

bool FrameListWidget::validKeyPress(Qt::Key key) {
    return (key & Qt::Key_Right) == Qt::Key_Right ||
           (key & Qt::Key_Left) == Qt::Key_Left;
}

void FrameListWidget::cloneItem(int fromGroup, Position fromPosition, Position toPosition) {
    FrameWidget* fromWidget;

    if(fromGroup == iGroupNumber) {
        fromWidget = static_cast<FrameWidget*>(&widgetAt(fromPosition));
    } else {
        FrameListWidget& group = static_cast<FrameListWidget&>(iGroupGroup.group(fromGroup));
        fromWidget = static_cast<FrameWidget*>(&group.widgetAt(fromPosition));
    }

    FrameWidget& toWidget = static_cast<FrameWidget&>(widgetAt(toPosition));

    toWidget.frame().setColour(fromWidget->frame().colour());
}

// events ------------------------------------

void FrameListWidget::resizeEvent(QResizeEvent *) {
    qDebug("FrameListWidget resize event");
    doResize();
}

void FrameListWidget::doResize() {
    if(!iResized) {
        qDebug("FrameListWidget doresize");
        iResized = true;
        int numFrames = iFramesList.count();

        if(numFrames > 0) {
            int extra = width()%numFrames;
            int frameWidth = (width()-extra)/numFrames;

            for(int i = 0; i < numFrames; i++) {
                iFramesList.at(i)->resize(frameWidth, height());
                iFramesList.at(i)->move(frameWidth*i + (10 - frameWidth/2), 0);
            }

            resize(width() - extra + (20 - frameWidth), height());

            emit resized();
        }
    } else {
        iResized = false;
    }
}
