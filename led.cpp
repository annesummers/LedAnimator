/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include "led.h"

#include "ledwidget.h"
#include "frame.h"
#include "animation.h"

#include "defaults.h"
#include "exceptions.h"

using namespace Exception;

Led::Led(QObject* parent, Animation &animation, int row, int column) :
    Selectable(parent),
    iRow(row),
    iColumn(column),
    iAnimation(animation),
    iSignalMapper(NULL),
    iDetailsNumber(INVALID){

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
        throw IllegalFrameNumberException("Frame number is zero or negative");
    }

    if(frameNum > iAnimation.numFrames()) {
        throw IllegalFrameNumberException("Frame number is greater than numFrames");
    }

    if(frameNum > iFrames.count()) {
        throw IllegalFrameNumberException("Frame number is greater than led number of frames");
    }
#endif

    return *(iFrames.at(frameNum - INITIAL_FRAME));
}

void Led::numFramesChanged(int numFrames) {
#ifndef NDEBUG
    if(numFrames <= 0) {
        throw IllegalArgumentException("numFrames is zero or negative");
    }

    if(numFrames > MAX_FRAMES) {
        throw IllegalArgumentException("numFrames is too big");
    }
#endif

    int oldNumFrames = iFrames.count();

    if(numFrames > oldNumFrames) {  // we need to add some frames
        for(int i = oldNumFrames; i < numFrames; i++) {
            Frame* frame = new Frame(this);
            iFrames.append(frame);

            iSignalMapper->setMapping(frame, i + INITIAL_FRAME);

            connect(frame, SIGNAL(colourChanged()), iSignalMapper, SLOT(map()));
        }
    } else if(numFrames <= iFrames.count()) { // we need to remove some frames; take them from the end
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
