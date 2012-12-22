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
    if(numRows <= 0 || numRows > MAX_ROWS) {
        throw IllegalArgumentException("Rows argument is invalid");
    }

    if(numColumns <= 0 || numColumns > MAX_COLUMNS) {
        throw IllegalArgumentException("Columns argument is invalid");
    }

    if(numFrames <= 0 || numFrames > MAX_FRAMES) {
        throw IllegalArgumentException("Frames argument is invalid");
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
    if(row >= numRows() || row < 0 || column >= numColumns() || column < 0 ) {
        throw InvalidStateException("Animation : led does not exist");
    }

    return *(iLeds.at((row*numColumns()) + column));
}

// slots --------

void Animation::nextFrame() {
    setCurrentFrame((currentFrame()%numFrames()) + INITIAL_FRAME);
}

// -------
