#include "TimeAxis.h"

#include "exceptions.h"

using namespace AnimatorModel;
using namespace Exception;

TimeAxis::TimeAxis(QObject *parent,
                   Animation& animation,
                   int lowValue,
                   int highValue,
                   int speed,
                   int priority,
                   bool isOpaque) :
    Axis(parent, animation, lowValue, highValue, lowValue, priority, isOpaque),
    iSpeed(speed),
    iIsPlaying(false),
    iRepeat(false),
    iPlayTimer(NULL) {
}


void TimeAxis::setLowValue(const int lowValue) {
    iLowValue = lowValue;

    emit lowValueChanged(lowValue);
}

void TimeAxis::setHighValue(const int highValue) {
    iHighValue = highValue;

    emit highValueChanged(highValue);
}

void TimeAxis::play(bool repeat) {
    if(!isPlaying()) {
        iRepeat = repeat;
        iPlayTimer = new QTimer(this);
        connect(iPlayTimer, SIGNAL(timeout()), this, SLOT(nextFrame()));
        iPlayTimer->start(speed());

        setPlaying(true);
    }
}

void TimeAxis::stop() {
    if(isPlaying()) {
        iPlayTimer->stop();
        delete iPlayTimer;
        iPlayTimer = NULL;

        setPlaying(false);

        emit stopped();
    }
}

void TimeAxis::nextFrame() {
    if(currentFrameNum() == highValue()) {
        if(iRepeat) {
            setCurrentFrame(lowValue());
        } else {
            stop();
        }
    } else {
        setCurrentFrame(currentFrameNum() + 1);
    }
}

TimeAxisData::TimeAxisData(QObject *parent,
                            Animation& animation,
                            TimeAxis& axis,
                           Led& led,
                           QUndoStack& undoStack) :
    AxisData(parent, animation, axis, led, undoStack) {
}

void TimeAxisData::lowValueChanged(const int lowValue) {

}

void TimeAxisData::highValueChanged(const int highValue){

}

void TimeAxisData::zeroValueChanged(const int zeroValue){

}
