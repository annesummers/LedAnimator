/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#include "led.h"

#include "ledwidget.h"
#include "frame.h"
#include "animation.h"

#include "constants.h"
#include "exceptions.h"

using namespace Exception;

Led::Led(QObject* parent, Animation &animation, int number, Position position) :
    GridItem(parent, animation, number, position),
    iSignalMapper(NULL),
    iGroupNumber(DEFAULT_GROUP),
    iHidden(false) {

#ifndef NDEBUG   
    if(!position.isValid()) {
        if(position.row() < 0) {
            throw IllegalArgumentException("Rows argument is invalid");
        }

        if(position.column() < 0) {
            throw IllegalArgumentException("Columns argument is invalid");
        }
    }
#endif

    iSignalMapper = new QSignalMapper(this);

    connect(&animation, SIGNAL(numFramesChanged(int)), this, SLOT(numFramesChanged(int)));
    connect(&(animation), SIGNAL(framesInserted(int,int)), this, SLOT(framesInserted(int, int)));
}

Led::~Led() {
}

Led::Led(const Led& copyLed) :
    GridItem(copyLed.parent(), copyLed.iAnimation, copyLed.number(), copyLed.position()),
    iSignalMapper(NULL),
    iGroupNumber(copyLed.iGroupNumber),
    iHidden(false) {

    iSignalMapper = new QSignalMapper(this);

    numFramesChanged(copyLed.iAnimation.numFrames());
    copyFrames(copyLed);

    connect(&iAnimation, SIGNAL(numFramesChanged(int)), this, SLOT(numFramesChanged(int)));
    connect(&iAnimation, SIGNAL(framesInserted(int,int)), this, SLOT(framesInserted(int, int)));
}

Led& Led::operator=(const Led& led) {
    setNumber(led.number());
    setPosition(led.position());
    setGroupNumber(led.groupNumber());

    numFramesChanged(led.iFrames.count());
    copyFrames(led);

    return *this;
}

Frame& Led::frameAt(int frameNum) const {
#ifndef NDEBUG
    if(frameNum < INITIAL_FRAME) {
        throw IllegalFrameNumberException("Led::frameAt : Frame number is zero or negative");
    }

    if(frameNum > iAnimation.numFrames()) {
        throw IllegalFrameNumberException("Led::frameAt : Frame number is greater than numFrames");
    }

    if(frameNum > iFrames.count()) {
        throw IllegalFrameNumberException("Led::frameAt : Frame number is greater than led number of frames");
    }
#endif

    return *(iFrames.at(frameNum - INITIAL_FRAME));
}

void Led::copyFrames(const Led &copyLed) {
    for(int i = INITIAL_FRAME; i < iAnimation.numFrames() + INITIAL_FRAME; i++) {
        frameAt(i).doSetColour(copyLed.frameAt(i).colour());
    }
}

void Led::numFramesChanged(int numFrames) {
#ifndef NDEBUG
    if(numFrames <= 0) {
        throw IllegalArgumentException("Led::numFramesChanged : numFrames is zero or negative");
    }

    if(numFrames > MAX_FRAMES) {
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
}

void Led::framesInserted(int numFrames, int numFramesAdded) {

}

void Led::colourChanged(int frameNum) {
    iAnimation.setSaved(false);

    if(frameNum == iAnimation.currentFrame()) {
        emit ledUpdated();
    }
}

void Led::setCurrentColour(QColor colour) {    
    frameAt(iAnimation.currentFrame()).setColour(colour);
}

void Led::move(Position newPosition) {
    setPosition(newPosition);
}

void Led::paste(Led& copyLed) {
    setCurrentColour(copyLed.currentColour());
}

void Led::setGroupNumber(int groupNumber) {
    iGroupNumber = groupNumber;

    if(iAnimation.numGroups() >= groupNumber) {
    }
}
