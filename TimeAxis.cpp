#include "TimeAxis.h"

#include "exceptions.h"

using namespace AnimatorModel;
using namespace Exception;

TimeAxis::TimeAxis(QObject *parent,
                   Animation& animation,
                   int lowValue,
                   int highValue,
                   int speed) :
    Axis(parent, animation, lowValue, highValue, lowValue),
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

/*void TimeAxisData::numFramesChanged(int numFrames) {
#ifndef NDEBUG
    if(numFrames <= 0) {
        throw IllegalArgumentException("Led::numFramesChanged : numFrames is zero or negative");
    }

    if(numFrames > iAxis.numFrames()) {
        throw IllegalArgumentException("Led::numFramesChanged : numFrames is too big");
    }
#endif

    int oldNumFrames = iFrames.count();

    if(numFrames > oldNumFrames) {  // we need to add some frames
        for(int i = oldNumFrames; i < numFrames; i++) {
            Frame* frame = new Frame(this, iAnimation, i + INITIAL_FRAME);
            iFrames.append(frame);

            iSignalMapper->setMapping(frame, i + INITIAL_FRAME);

            connect(frame, SIGNAL(colourChanged()), iSignalMapper, SLOT(map()));
        }
    } else if(numFrames < iFrames.count()) { // we need to remove some frames; take them from the end
        for(int i = oldNumFrames; i <= numFrames; i--) {
            Frame* frame = iFrames.at(i);

            iFrames.removeAt(i);
            delete frame;
        }
    }

    if(oldNumFrames != numFrames) {
        // do I have to reconnect?
        connect(iSignalMapper, SIGNAL(mapped(int)), this, SLOT(colourChanged(int)));
    }
}*/

//void Led::framesInserted(int numFrames, int numFramesAdded) {

//}

