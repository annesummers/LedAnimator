#include "Axis.h"
#include "exceptions.h"

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
    return iHighValue - iLowValue;
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

AxisData::AxisData(QObject *parent,
                   Animation &animation,
                      Axis& axis,
                   QUndoStack &undoStack) :
    QObject(parent),
    iAxis(axis),
    iAnimation(animation),
    iUndoStack(undoStack){

}

