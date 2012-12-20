#include "animation.h"

#include "led.h"
#include "mainwindow.h"
#include "ledwidgets.h"

#include "defaults.h"
#include "exceptions.h"

#include <QtDebug>

Animation::Animation(Engine& engine) :
    QObject(&engine),
    iEngine(engine),
    iNumRows(0),
    iNumColumns(0),
    iNumFrames(0),
    iCurrentFrame(1),
    iFrameFrequency(DEFAULT_FRAME_FREQUENCY),
    iIsPlaying(false),
    iFileName(""),
    iIsSaved(false),
    iPlayTimer(NULL) { }

void Animation::setupNew(int numRows, int numColumns, int numFrames) {
    iNumRows = numRows;
    iNumColumns = numColumns;
    iNumFrames = numFrames;
    iCurrentFrame = 1;
    iIsSaved = false;

    Led* led = NULL;
    foreach(led, iLeds) {
        delete led;
        led = NULL;
    }
    iLeds.clear();

    for(int i = 0; i < iNumColumns; i++) {
        for(int j = 0; j < iNumRows; j++) {
            led = new Led(*this, j, i);
            iLeds.append(led);

            emit newLed(*led);
        }
    }

    emit currentFrameChanged(iCurrentFrame);
    emit numFramesChanged(iNumFrames);
}

int Animation::currentFrame() {
    return iCurrentFrame;
}

void Animation::setCurrentFrame(int frame) {
    iCurrentFrame = frame;

    emit currentFrameChanged(iCurrentFrame);
}

int Animation::numFrames() {
    return iNumFrames;
}

void Animation::setNumFrames(int numFrames) {
    iNumFrames = numFrames;

    emit numFramesChanged(iNumFrames);
}

int Animation::frameFrequency() {
    return iFrameFrequency;
}

void Animation::setFrameFrequency(int frameFrequency) {
    iFrameFrequency = frameFrequency;
}

void Animation::play() {
    if(!iIsPlaying) {
        iPlayTimer = new QTimer(this);
        connect(iPlayTimer, SIGNAL(timeout()), this, SLOT(nextFrame()));
        iPlayTimer->start(iFrameFrequency);

        iIsPlaying = true;
    }
}

void Animation::stop() {
    if(iIsPlaying) {
        iPlayTimer->stop();
        delete iPlayTimer;
        iPlayTimer = NULL;

        iIsPlaying = false;
    }
}

bool Animation::isPlaying() {
    return iIsPlaying;
}

int Animation::numRows() {
    return iNumRows;
}

int Animation::numColumns() {
    return iNumColumns;
}

int Animation::numLeds() {
    return iLeds.count();
}

Led& Animation::ledAt(int row, int column) {
    if(row >= iNumRows || row < 0 || column >= iNumColumns || column < 0 ) {
        throw new InvalidStateException("Animation : led does not exist");
    }

    return *(iLeds.at((row*iNumColumns) + column));
}

QString Animation::fileName() {
    return iFileName;
}

void Animation::setFileName(QString fileName) {
    iFileName = fileName;
}

bool Animation::saved() {
    return iIsSaved;
}

void Animation::setSaved(bool saved) {
    iIsSaved = saved;
}

// slots --------

void Animation::nextFrame() {
    setCurrentFrame(iCurrentFrame);

    iCurrentFrame++;
    if(iCurrentFrame >= iNumFrames) {
        iCurrentFrame = 1;  // frames are indexed from 1
    }
}

// -------
