#include "framelistwidget.h"

#include "Animation.h"
#include "Led.h"
#include "framewidget.h"
#include "selectablegroupgroupwidget.h"

#include "exceptions.h"

#include "fadecalculator.h"

using namespace Exception;
using namespace AnimatorUi;

FrameListWidget::FrameListWidget(QWidget *parent,
                                 const AxisData &axisData,
                                 ColourGroupGroupWidget &framesListGroup,
                                 int groupNumber) :
    ColourGroupWidget(parent, 0, axisData.numFrames(), framesListGroup, groupNumber),
    iAxis(axisData),
    iResized(false) {

    //numFramesChanged(axis.numFrames());
    //lowValueChanged(axisData.axis().lowValue());
    highValueChanged(axisData.axis().highValue());

    setFocusPolicy(Qt::ClickFocus);

    connect(&axisData.axis(), SIGNAL(lowValueChanged(int)), this, SLOT(lowValueChanged(int)));
    connect(&axisData.axis(), SIGNAL(highValueChanged(int)), this, SLOT(highValueChanged(int)));
    connect(&axisData.axis(), SIGNAL(framesInserted(int, int)), this, SLOT(framesInserted(int, int)));
}

// slots --------------------

void FrameListWidget::lowValueChanged(int lowValue) {
    int oldNumFrames = iFramesList.count();

    if(iAxis.numFrames() > oldNumFrames) {  // we need to add more frames
        for(int i = oldNumFrames; i < iAxis.numFrames(); i++) {
            FrameWidget* frame = new FrameWidget(this, *this, iAxis.frameAt(iAxis.axis().lowValue() + i));
            iFramesList.append(frame);
        }
    } else if (iAxis.numFrames() < oldNumFrames ) {  // we need to remove some frames; take them from the start
        for(int i = 0; i < oldNumFrames - iAxis.numFrames(); i++) {
            FrameWidget* frame = iFramesList.at(i);

            iFramesList.removeAt(i);
            delete frame;
        }
    }

    setSize();
}

void FrameListWidget::setSize() {
    setMinimumWidth(iAxis.numFrames() * 7);
    setMaximumWidth(iAxis.numFrames() * 7);

    doResize();
}

void FrameListWidget::highValueChanged(int highValue) {
    int oldNumFrames = iFramesList.count();

    if(iAxis.numFrames() > oldNumFrames) {  // we need to add more frames
        for(int i = iAxis.numFrames() - oldNumFrames; i > 0; i--) {
            FrameWidget* frame = new FrameWidget(this, *this, iAxis.frameAt(iAxis.axis().highValue() - i));
            iFramesList.append(frame);
        }
    } else if (iAxis.numFrames() < oldNumFrames ) {  // we need to remove some frames; take them from the end
        for(int i = oldNumFrames; i >= iAxis.numFrames(); i--) {
            FrameWidget* frame = iFramesList.at(i);

            iFramesList.removeAt(i);
            delete frame;
        }
    }

    setSize();
}

void FrameListWidget::framesInserted(int numFrames, int framesAdded) {

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

    toWidget.frame().setValue(fromWidget->frame().value());
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
