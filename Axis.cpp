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

    emit lowValueChanged(iLowValue);
    emit highValueChanged(iHighValue);
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
