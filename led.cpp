/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include "led.h"

#include "ledwidgets.h"
#include "frame.h"
#include "animation.h"

#include "defaults.h"
#include "exceptions.h"

using namespace Exception;

Led::Led(QObject* parent, const Animation &animation, int row, int column) :
    QObject(parent),
    iAnimation(animation),
    iSignalMapper(NULL),
    iRow(row),
    iColumn(column),
    iIsSelected(false) {

    if(row < 0) {
        throw IllegalArgumentException("Rows argument is invalid");
    }

    if(column < 0) {
        throw IllegalArgumentException("Columns argument is invalid");
    }

    iSignalMapper = new QSignalMapper(this);

    QObject::connect(&animation, SIGNAL(numFramesChanged(int)), this, SLOT(numFramesChanged(int)));
}

Frame& Led::frameAt(int frameNum) const {
    if(frameNum < INITIAL_FRAME) {
        throw IllegalFrameNumberException("Frame number is zero or negative");
    }

    if(frameNum > iAnimation.numFrames()) {
        throw IllegalFrameNumberException("Frame number is greater than numFrames");
    }

    if(frameNum > iFrames.count()) {
        throw IllegalFrameNumberException("Frame number is greater than led number of frames");
    }

    return *(iFrames.at(frameNum - INITIAL_FRAME));
}

void Led::numFramesChanged(int numFrames) {
    int oldNumFrames = iFrames.count();

    if(numFrames > oldNumFrames) {  // we need to add more frames
        for(int i = oldNumFrames; i < numFrames; i++) {
            Frame* frame = new Frame(this);
            iFrames.append(frame);

            iSignalMapper->setMapping(frame, i + INITIAL_FRAME);

            connect(frame, SIGNAL(colourChanged()), iSignalMapper, SLOT(map()));
        }
    } else {
        for(int i = oldNumFrames; i > numFrames; i--) {
            Frame *frame = iFrames.at(i);

            iSignalMapper->removeMappings(frame);
            iFrames.removeAt(i);

            delete frame;
        }
    }

    if(oldNumFrames != 0) {
        disconnect(iSignalMapper, SIGNAL(mapped(int)), this, SLOT(colourChanged(int)));
    }

    connect(iSignalMapper, SIGNAL(mapped(int)), this, SLOT(colourChanged(int)));

    // TODO hmm, more elegant, probably slower though
}

void Led::colourChanged(int frameNum) {
    if(frameNum + INITIAL_FRAME == iAnimation.currentFrame()) {
        emit currentColourChanged();
    }
}

void Led::setCurrentColour(QColor colour) {    
    frameAt(iAnimation.currentFrame()).setColour(colour);  // frames are indexed from 1!
    colourChanged(iAnimation.currentFrame());
}

void Led::select(bool isSelected) {
    iIsSelected = isSelected;

    emit selected();
}
