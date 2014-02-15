#include "framelistwidget.h"

#include <QtCore/qmath.h>

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
                                 ColourGroupGroupWidget &framesListGroup//,
                                 /*int groupNumber*/) :
    ColourGroupWidget(parent, 0, axisData.axis().numFrames(), framesListGroup),//, groupNumber),
    iAxisData(axisData),
    iResized(false) {

    // create the zero widget
    FrameWidget* frame = new FrameWidget(this, *this, iAxisData.frameAt(iAxisData.axis().zeroValue()));
    iFramesList.insert(0, frame);
    connect(&frame->frame(), SIGNAL(updated()), this, SLOT(update()));

    handleLowValueChanged(0, axisData.axis().lowValue());
    handleHighValueChanged(0, axisData.axis().highValue());

    setFocusPolicy(Qt::ClickFocus);

    connect(&axisData.axis(), SIGNAL(handleLowValueChanged(int)), this, SLOT(handleLowValueChanged(int)));
    connect(&axisData.axis(), SIGNAL(handleHighValueChanged(int)), this, SLOT(handleHighValueChanged(int)));
    connect(&axisData.axis(), SIGNAL(handleFramesInserted(int, int)), this, SLOT(handleFramesInserted(int, int)));
}

// slots --------------------

void FrameListWidget::handleLowValueChanged(int oldLowValue, int lowValue) {
    if(lowValue < oldLowValue) {  // we need to add more frames
        for(int i = 0; i <  qAbs(lowValue - oldLowValue); i++) {
            FrameWidget* frame = new FrameWidget(this, *this, iAxisData.frameAt(iAxisData.axis().lowValue() + i));
            iFramesList.insert(i, frame);
            connect(&frame->frame(), SIGNAL(updated()), this, SLOT(update()));
        }
    } else if (lowValue > oldLowValue ) {  // we need to remove some frames; take them from the start
        for(int i = 0; i <  qAbs(oldLowValue - lowValue); i++) {
            FrameWidget* frame = iFramesList.at(0);

            iFramesList.removeAt(0);
            delete frame;
        }
    }

    setSize();
}

void FrameListWidget::setSize() {
    setMinimumWidth((iAxisData.axis().numFrames() + 2) * 7);
    setMaximumWidth((iAxisData.axis().numFrames() + 2) * 7);

    doResize();
}

void FrameListWidget::handleHighValueChanged(int oldHighValue, int highValue) {
    if(highValue > oldHighValue) {  // we need to add more frames
        for(int i = qAbs(highValue - oldHighValue) - 1; i >= 0 ; i--) {
            FrameWidget* frame = new FrameWidget(this, *this, iAxisData.frameAt(iAxisData.axis().highValue() - i));
            iFramesList.append(frame);
            connect(&frame->frame(), SIGNAL(updated()), this, SLOT(update()));
        }
    } else if (highValue < oldHighValue ) {  // we need to remove some frames; take them from the end
        for(int i = qAbs(oldHighValue - highValue); i >= 0; i--) {
            FrameWidget* frame = iFramesList.at(iFramesList.count() - 1);

            iFramesList.removeAt(iFramesList.count() - 1);
            delete frame;
        }
    }

    setSize();
}

void FrameListWidget::handleFramesInserted(int numFrames, int framesAdded) {
    Q_UNUSED(numFrames);
    Q_UNUSED(framesAdded);
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
                iFramesList.at(i)->move(frameWidth*(i), 0);
            }

            resize(frameWidth * (numFrames+2), height());//width() - extra + (20 - frameWidth), height());

            emit resized();
        }
    } else {
        iResized = false;
    }
}
