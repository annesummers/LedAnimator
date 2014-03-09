#include "Axis.h"
#include "exceptions.h"
#include "Led.h"
#include "Animation.h"
#include "FunctionValue.h"

using namespace AnimatorModel;
using namespace Exception;

Axis::Axis(QObject *parent,
           Animation &animation,
           int lowValue,
           int highValue,
           int zeroValue,
           int priority,
           bool isOpaque) :
    QObject(parent),
    iLowValue(lowValue),
    iHighValue(highValue),
    iZeroValue(zeroValue),
    iAnimation(animation),
    iCurrentFrame(0),
    iOpaque(isOpaque),
    iPriority(priority) {

    if(iLowValue > iHighValue) {
        throw IllegalArgumentException("Axis:Axis : low value is greater than high value");
    }

    if(iZeroValue < iLowValue) {
        throw IllegalArgumentException("Axis:Axis : zero value is smaller than low value");
    }

    if(iZeroValue > iHighValue) {
        throw IllegalArgumentException("Axis:Axis : zero value is greater than high value");
    }

    if(iPriority > kPriorityHigh) {
        throw IllegalArgumentException("Axis:Axis : priority is greater than highest priority");
    }

    if(iPriority < kPriorityLow) {
        throw IllegalArgumentException("Axis:Axis : priority is smaller than lowest priority");
    }
}

int Axis::numFrames() const {
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

void Axis::setLowValue(const int lowValue) {
    int oldLowValue = iLowValue;

    iLowValue = lowValue;

    if(iZeroValue < iLowValue) {
        throw IllegalArgumentException("Axis:setLowValue : zero value is smaller than low value");
    }

    emit lowValueChanged(oldLowValue, iLowValue);
}

void Axis::setHighValue(const int highValue) {
    int oldHighValue = iHighValue;

    iHighValue = highValue;

    if(iZeroValue > iHighValue) {
        throw IllegalArgumentException("Axis:setHighValue : zero value is greater than high value");
    }

    emit highValueChanged(oldHighValue, iHighValue);
}

