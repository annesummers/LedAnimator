/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include "animation.h"

#include "led.h"
#include "mainwindow.h"
#include "ledwidgets.h"

#include "defaults.h"
#include "exceptions.h"

#include <QtDebug>

using namespace Exception;

Animation::Animation(Engine& engine) :
    QObject(&engine),
    iEngine(engine),
    iPlayTimer(NULL),
    iNumRows(INVALID),
    iNumColumns(INVALID),
    iNumFrames(INVALID),
    iCurrentFrame(INVALID),
    iFrameFrequency(DEFAULT_FRAME_FREQUENCY),
    iIsPlaying(false),
    iFileName(""),
    iIsSaved(false){ }

void Animation::setupNew(int numRows, int numColumns, int numFrames) {
    if(numRows <= 0) {
        throw IllegalArgumentException("numRows is zero or negative");
    }

    if(numRows > MAX_ROWS){
        throw IllegalArgumentException("numRows is too big");
    }

    if(numColumns <= 0) {
        throw IllegalArgumentException("numColumns is zero or negative");
    }

    if(numColumns > MAX_COLUMNS) {
        throw IllegalArgumentException("numColumns is too big");
    }

    if(numFrames <= 0) {
        throw IllegalArgumentException("numFrames is zero or negative");
    }

    if(numFrames > MAX_FRAMES) {
        throw IllegalArgumentException("numFrames is too big");
    }

    Led* led = NULL;
    foreach(led, iLeds) {
        delete led;
        led = NULL;
    }
    iLeds.clear();

    setNumRows(numRows);
    setNumColumns(numColumns);

    for(int i = 0; i < numRows; i++) {
        for(int j = 0; j < numColumns; j++) {
            led = new Led(this, *this, j, i);
            iLeds.append(led);

            emit newLed(i, j);
        }
    }  

    setNumFrames(numFrames);
    setCurrentFrame(INITIAL_FRAME);
    setSaved(false);
}

void Animation::play() {
    if(!isPlaying()) {
        iPlayTimer = new QTimer(this);
        connect(iPlayTimer, SIGNAL(timeout()), this, SLOT(nextFrame()));
        iPlayTimer->start(frameFrequency());

        setPlaying(true);
    }
}

void Animation::stop() {
    if(isPlaying()) {
        iPlayTimer->stop();
        delete iPlayTimer;
        iPlayTimer = NULL;

        setPlaying(false);
    }
}

Led &Animation::ledAt(int row, int column) const {
    if(row >= numRows()) {
        throw IllegalArgumentException("Row is greater than number of rows");
    }

    if(row < 0) {
        throw IllegalArgumentException("Row is negative");
    }

    if(column >= numColumns()) {
        throw IllegalArgumentException("Column is greater than number of columns");
     }

    if(column < 0 ) {
        throw IllegalArgumentException("Column is negative");
    }

    return *(iLeds.at((row*numColumns()) + column));
}

void Animation::setCurrentFrame(int frame) {
    if(frame < INITIAL_FRAME) {
        throw IllegalArgumentException("Frame number is smaller than first frame");
    }

    if(frame > numFrames()) {
        throw IllegalArgumentException("Frame number is greater than last frame");
    }

    iCurrentFrame = frame;
    emit currentFrameChanged(iCurrentFrame);
}

void Animation::setFrameFrequency(int frameFrequency) {
    iFrameFrequency = frameFrequency;
}


// slots --------

void Animation::nextFrame() {
    setCurrentFrame((currentFrame()%numFrames()) + INITIAL_FRAME);
}

// -------
