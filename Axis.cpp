#include "Axis.h"
#include "exceptions.h"
#include "Led.h"

using namespace AnimatorModel;
using namespace Exception;

Axis::Axis(QObject *parent,
           Animation &animation,
           int lowValue,
           int highValue,
           int zeroValue) :
    QObject(parent),
    iLowValue(lowValue),
    iHighValue(highValue),
    iZeroValue(zeroValue),
    iAnimation(animation),
    iCurrentFrame(0) {

    emit lowValueChanged(iLowValue);
    emit highValueChanged(iHighValue);
}

const int Axis::numFrames() const {
    return iHighValue - iLowValue + 1;
}

void Axis::setCurrentFrame(int frame) {
    if(frame < iLowValue) {
        throw IllegalArgumentException("Axis::setCurrentFrame : Frame number is smaller than first frame");
    }

    if(frame > iHighValue) {
        throw IllegalArgumentException("Axis::setCurrentFrame : Frame number is greater than last frame");
    }

    iCurrentFrame = frame;
    emit currentFrameChanged(iCurrentFrame);
}

/*const Frame& Axis::currentFrame() const {
    return frameAt(iCurrentFrame);
}*/

AxisData::AxisData(QObject *parent,
                   Animation &animation,
                   Axis& axis,
                   Led &led,
                   QUndoStack &undoStack) :
    QObject(parent),
    iAxis(axis),
    iAnimation(animation),
    iUndoStack(undoStack),
    iSignalMapper(NULL) {

    iSignalMapper = new QSignalMapper(this);

    Frame* newFrame = new Frame(this, animation, axis.lowValue(), NULL, undoStack);
    Frame* previousFrame = NULL;
    for(int i = axis.lowValue() + 1; i <= axis.highValue(); i++) {
        iFrames.insert(newFrame->number(), newFrame);

        iSignalMapper->setMapping(newFrame, newFrame->number());
        connect(newFrame, SIGNAL(valueChanged()), iSignalMapper, SLOT(map()));

        previousFrame = newFrame;
        newFrame = new Frame(this, animation, i, previousFrame, undoStack);
        previousFrame->setNext(*newFrame);
    }

    iFrames.insert(newFrame->number(), newFrame);

    connect(iSignalMapper, SIGNAL(mapped(int)), &led, SLOT(colourChanged(int)));
}

void AxisData::copyFrames(const AxisData &copyAxis) {
    for(int i = iAxis.lowValue(); i < iAxis.highValue(); i++) {
        frameAt(i).doSetValue(copyAxis.frameAt(i).value());
    }
}

