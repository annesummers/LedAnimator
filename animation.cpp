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

#include "constants.h"
#include "exceptions.h"

#include <QtDebug>

using namespace Exception;
using namespace ImportExport;
using namespace AnimatorModel;

Animation::Animation(Engine& engine) :
    QObject(&engine),
    iEngine(engine),
    iPlayTimer(NULL),
    iNumRows(INVALID),
    iNumColumns(INVALID),
    iNumLeds(INVALID),
    iNumFrames(INVALID),
    iCurrentFrame(INVALID),
    iFrameFrequency(DEFAULT_FRAME_FREQUENCY),
    iIsPlaying(false),
    iFileName(""),
    iIsSaved(false){ }

void Animation::setupNew(int numRows, int numColumns, int numFrames, int frameFrequency, int numLeds, QList<int> ledPositions) {
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

    setNumLeds(numLeds);

    setNumFrames(numFrames);
    setCurrentFrame(INITIAL_FRAME);
    setFrameFrequency(frameFrequency);

    setSaved(false);
    setFileName("");

    for(int i = 0; i < numRows; i++) {
        for(int j = 0; j < numColumns; j++) {
            emit newSocket(i, j);
        }
    }

    iPositions = ledPositions;

    int row;
    int column;

    for(int i = 0; i < numLeds; i++) {
        getLedPosition(i, &row, &column);
        addNewLed(row, column);
    }

    const QClipboard *clipboard = QApplication::clipboard();

    if(clipboard->mimeData()->hasFormat(LED_MIME_TYPE) ||
       clipboard->mimeData()->hasFormat(FRAME_MIME_TYPE)) {
        QApplication::clipboard()->clear();
    }
}

void Animation::addNewLed(int row, int column) {
    Led* led = new Led(this, *this, ++iNumLeds, row, column);

    addLed(*led, row, column);
}

void Animation::addLed(Led& led, int row, int column) {
    iLeds.insert(iNumLeds, &led);

    setGridPositionNumber(row, column, iNumLeds);

    led.numFramesChanged(iNumFrames);

    emit newLed(row, column);
}

void Animation::moveLed(Led& led, int toRow, int toColumn) {
    setGridPositionNumber(led.row(), led.column(), INVALID);
    setGridPositionNumber(toRow, toColumn, led.number());
}

void Animation::copyLed(Led& led, int toRow, int toColumn) {
    Led* newLed = new Led(this, *this, ++iNumLeds, toRow, toColumn);
    addLed(*newLed, toRow, toColumn);
    newLed->copyFrames(led);
}

void Animation::deleteLed(Led& led) {
    iLeds.remove(led.number());
    setGridPositionNumber(led.row(), led.column(), INVALID);
   // delete &led;
    // TODO why does this fail?
}

void Animation::renumberLed(Led& led, int newNumber) {
    iLeds.remove(led.number());
    iLeds.insert(newNumber, &led);

    setGridPositionNumber(led.row(), led.column(), newNumber);

    led.setNumber(newNumber);
}

void Animation::setupNew(int numRows, int numColumns, int numFrames, int frameFrequency) {
    setupNew(numRows, numColumns, numFrames, frameFrequency, 0, QList<int>());
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

Led *Animation::ledAt(int row, int column) const {
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

    int ledNumber = getLedNumber(row, column);
    if(ledNumber != INVALID) {
        return &ledAt(ledNumber);
    }

    return NULL;
}

Led& Animation::ledAt(int number) const {
    if(iLeds.contains(number)) {
        return *iLeds.value(number);
    } else {
        throw IllegalArgumentException(QString("Animation::ledAt : led %1 does not exist").arg(number));
    }
}

int Animation::gridPositionNumber(int row, int column) const {
    return (row*numColumns()) + column;
}

void Animation::getGridPosition(int index, int* row, int* column) const {
    *row = index/numRows();
    *column = index%numRows();
}

void Animation::setGridPositionNumber(int row, int column, int number) {
    iPositions[gridPositionNumber(row, column)] = number;
}

void Animation::getLedPosition(int number, int *const row, int *const column)  const {
    getGridPosition(iPositions.indexOf(number), row, column);
}

const int Animation::getLedNumber(int row, int column) const {
    int gridPositionNum = gridPositionNumber(row, column);
    return iPositions.at(gridPositionNum);
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
