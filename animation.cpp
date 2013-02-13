/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include "animation.h"

#include "led.h"
#include "mainwindow.h"
#include "ledwidget.h"
#include "ledanimcodec.h"

#include "defaults.h"
#include "exceptions.h"

#include <QtDebug>

using namespace Exception;
using namespace ImportExport;

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

void Animation::setupNew(int numRows, int numColumns, int numFrames, int frameFrequency) {
    if(numRows <= 0) {
        throw IllegalArgumentException("Animation::setupNew : numRows is zero or negative");
    }

    if(numRows > MAX_ROWS){
        throw IllegalArgumentException("Animation::setupNew : numRows is too big");
    }

    if(numColumns <= 0) {
        throw IllegalArgumentException("Animation::setupNew : numColumns is zero or negative");
    }

    if(numColumns > MAX_COLUMNS) {
        throw IllegalArgumentException("Animation::setupNew : numColumns is too big");
    }

    if(numFrames <= 0) {
        throw IllegalArgumentException("Animation::setupNew : numFrames is zero or negative");
    }

    if(numFrames > MAX_FRAMES) {
        throw IllegalArgumentException("Animation::setupNew : numFrames is too big");
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
            led = new Led(this, *this, i, j);
            iLeds.append(led);

            emit newLed(i, j);
        }
    }  

    setNumFrames(numFrames);
    setCurrentFrame(INITIAL_FRAME);
    setFrameFrequency(frameFrequency);

    setSaved(false);
    setFileName("");
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
        throw IllegalArgumentException("Animation::ledAt : Row is greater than number of rows");
    }

    if(row < 0) {
        throw IllegalArgumentException("Animation::ledAt : Row is negative");
    }

    if(column >= numColumns()) {
        throw IllegalArgumentException("Animation::ledAt : Column is greater than number of columns");
     }

    if(column < 0 ) {
        throw IllegalArgumentException("Animation::ledAt : Column is negative");
    }

    return *(iLeds.at((row*numColumns()) + column));
}

// slots -------------------------

void Animation::setCurrentFrame(int frame) {
    if(frame < INITIAL_FRAME) {
        throw IllegalArgumentException("Animation::setCurrentFrame : Frame number is smaller than first frame");
    }

    if(frame > numFrames()) {
        throw IllegalArgumentException("Animation::setCurrentFrame : Frame number is greater than last frame");
    }

    iCurrentFrame = frame;
    emit currentFrameChanged(iCurrentFrame);
}

void Animation::setFrameFrequency(int frameFrequency) {
    iFrameFrequency = frameFrequency;
}

void Animation::nextFrame() {
    setCurrentFrame((currentFrame()%numFrames()) + INITIAL_FRAME);
}

void Animation::copyToClipboard() {
    LedAnimStringCodec codec(*this);
    codec.writeAnimation();

    QClipboard *clipboard = QApplication::clipboard();
    QMimeData *data = new QMimeData;

    data->setData("text/plain", codec.asString().toUtf8());
    clipboard->setMimeData(data);
}
