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

Led::Led(QObject* parent, Animation &animation, int number, int row, int column) :
    GridItem(parent, number, row, column),
    iAnimation(animation),
    iSignalMapper(NULL),
    iGroupNumber(DEFAULT_GROUP),
    iHidden(false) {

#ifndef NDEBUG
    if(row < 0) {
        throw IllegalArgumentException("Rows argument is invalid");
    }

    if(column < 0) {
        throw IllegalArgumentException("Columns argument is invalid");
    }
#endif

    iSignalMapper = new QSignalMapper(this);

    connect(&animation, SIGNAL(numFramesChanged(int)), this, SLOT(numFramesChanged(int)));
}

Led::~Led() {
    Frame* frame = NULL;
    foreach(frame, iFrames) {
        disconnect(frame, 0, this, 0);
    }

    disconnect(iSignalMapper, 0, this, 0);

    disconnect(&iAnimation, 0, this, 0);
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

void Led::copyFrames(Led& copyLed) {
    for(int i = INITIAL_FRAME; i < iAnimation.numFrames() + INITIAL_FRAME; i++) {
        frameAt(i).setColour(copyLed.frameAt(i).colour());
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
            Frame* frame = new Frame(this, i + INITIAL_FRAME);
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

void Led::colourChanged(int frameNum) {
    iAnimation.setSaved(false);

    if(frameNum == iAnimation.currentFrame()) {
        emit ledUpdated();
    }
}

void Led::setCurrentColour(QColor colour) {    
    frameAt(iAnimation.currentFrame()).setColour(colour);
}

void Led::move(int newRow, int newColumn) {
    setRow(newRow);
    setColumn(newColumn);
}

void Led::paste(Led& copyLed) {
    setCurrentColour(copyLed.currentColour());
}

void Led::setGroupNumber(int groupNumber) {
    iGroupNumber = groupNumber;

    if(iAnimation.numGroups() >= groupNumber) {
    }
}
