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
    iNumFrames(INVALID),
    iCurrentFrame(INVALID),
    iFrameFrequency(DEFAULT_FRAME_FREQUENCY),
    iNumSubAnimations(1),
    iIsPlaying(false),
    iRepeat(false),
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

    if(ledPositions.count() != 0) {
        iPositions = ledPositions;
    } else {
        iPositions.clear();
        for(int i = 0; i < numRows * numColumns; i++) {
            iPositions.append(INVALID);
        }
    }

    int ledNum = INITIAL_LED;

    iMissingLeds.clear();
    for(int i = 0; i < numLeds; i++) {
        Position position;
        do {
            position = ledPosition(ledNum);
            if(!position.isValid()) {
                iMissingLeds.append(ledNum++);
            }
        } while(!position.isValid());

        addNewLed(position.row(), position.column(), ledNum++);
    }

    iGreatestNumber = ledNum - 1;

    const QClipboard *clipboard = QApplication::clipboard();

    if(clipboard->mimeData()->hasFormat(LED_MIME_TYPE) ||
       clipboard->mimeData()->hasFormat(FRAME_MIME_TYPE)) {
        QApplication::clipboard()->clear();
    }

    iClipboardLeds.clear();
    iClipboardPositions.clear();
    for(int i = 0; i < numRows * numColumns; i++) {
        iClipboardPositions.append(INVALID);
    }
}

void Animation::setupNew(int numRows, int numColumns, int numFrames, int frameFrequency) {
    setupNew(numRows, numColumns, numFrames, frameFrequency, 0, QList<int>());
}

void Animation::addNewLed(int row, int column, int ledNum) {
    if(row < 0) {
        throw IllegalArgumentException("Animation::addNewLed : row is negative");
    }

    if(column < 0) {
        throw IllegalArgumentException("Animation::addNewLed : column is negative");
    }

    if(row >= numRows()) {
        throw IllegalArgumentException("Animation::addNewLed : row is bigger than num rows");
    }

    if(column >= numColumns()) {
        throw IllegalArgumentException("Animation::addNewLed : column is bigger than num columns");
    }

    Led* led = new Led(this, *this, ledNum != INVALID ? ledNum : nextLedNumber(), row, column);
    addLed(*led, row, column);
}

void Animation::addLed(Led& led, int row, int column) {
    addLed(led, led.number());

    led.numFramesChanged(iNumFrames);

    emit newLed(row, column);
}

void Animation::deleteLed(int row, int column) {
    Led& led = *ledAt(row, column);
    int number = led.number();

    removeLed(led);

    emit ledDeleted(row, column, number);

   // delete &led;
    // TODO why does this fail?
}

void Animation::moveLed(int fromRow, int fromColumn, int toRow, int toColumn) {
    Led& led = *ledAt(fromRow, fromColumn);
    Led* toLed = ledAt(toRow, toColumn);

    if(toLed != NULL) {
        return;
        //deleteLed(toRow, toColumn);
    }

    setGridPositionNumber(fromRow, fromColumn, INVALID);
    setGridPositionNumber(toRow, toColumn, led.number());

    led.move(toRow, toColumn);

    emit ledMoved(fromRow, fromColumn, toRow, toColumn);
}

void Animation::cloneLed(int fromRow, int fromColumn, int toRow, int toColumn) {
    Led* newLed = ledAt(toRow, toColumn);
    if(newLed == NULL) {
        newLed = new Led(this, *this, nextLedNumber(), toRow, toColumn);
        addLed(*newLed, toRow, toColumn);
    }

    newLed->copyFrames(*ledAt(fromRow, fromColumn));
}

void Animation::moveLedToClipboard(int row, int column) {
    Led* led = ledAt(row, column);

    setClipboardGridPositionNumber(row, column, led->number());
    iClipboardLeds.insert(led->number(), led);

    deleteLed(row, column);
}

void Animation::pasteLed(int fromRow, int fromColumn, int toRow, int toColumn) {
    if(fromRow >= numRows()) {
        throw IllegalArgumentException("Animation::pasteLed : Row is greater than number of rows");
    }

    if(fromRow < 0) {
        throw IllegalArgumentException("Animation::pasteLed : Row is negative");
    }

    if(fromColumn >= numColumns()) {
        throw IllegalArgumentException("Animation::pasteLed : Column is greater than number of columns");
     }

    if(fromColumn < 0 ) {
        throw IllegalArgumentException("Animation::pasteLed : Column is negative");
    }

    int ledNumber = iClipboardPositions.at(gridPositionNumber(fromRow, fromColumn));
    if(!iClipboardLeds.contains(ledNumber)) {
        throw IllegalArgumentException("Animation::pasteLed : Led is NULL");
    }

    Led* led = iClipboardLeds.value(ledNumber);

    setClipboardGridPositionNumber(fromRow, fromColumn, INVALID);
    iClipboardLeds.remove(ledNumber);

    led->move(toRow, toColumn);
    addLed(*led, toRow, toColumn);
}

void Animation::renumberLed(int row, int column, int newNumber) {
    Led& led = *ledAt(row, column);

    int oldNumber = led.number();

    removeLed(led);
    addLed(led, newNumber);

    led.setNumber(newNumber);

    emit ledRenumbered(row, column, oldNumber);
}

int Animation::nextLedNumber() {
    return ++iGreatestNumber;
}

void Animation::addLed(Led& led, int number) {
    iLeds.insert(number, &led);

    if(number > iGreatestNumber) {
        iGreatestNumber = number;
    }

    if(isMissing(number)) {
        iMissingLeds.removeOne(number);
    }

    setGridPositionNumber(led.row(), led.column(), led.number());
}

void Animation::removeLed(Led& led) {
    iLeds.remove(led.number());

    if(led.number() == iGreatestNumber) {
        iGreatestNumber--;
    } else {
        iMissingLeds.append(led.number());
    }

    setGridPositionNumber(led.row(), led.column(), INVALID);
}

void Animation::selectGroup(int groupNumber) {
    Led* led;
    foreach(led, iLeds) {
        if(led->groupNumber() == groupNumber) {
            led->select(true);
        } else {
            led->select(false);
        }
    }
}

void Animation::play(bool repeat) {
    if(!isPlaying()) {
        iRepeat = repeat;
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

        emit stopped();
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

    int ledNumber = iPositions.at(gridPositionNumber(row, column));

    return ledAt(ledNumber);
}

Led* Animation::ledAt(int number) const {
    if(iLeds.contains(number)) {
        return iLeds.value(number);
    } else {
       // throw IllegalArgumentException(QString("Animation::ledAt : led %1 does not exist").arg(number));
        return NULL;
    }
}

Position Animation::ledPosition(int number) const {//, int *const row, int *const column)  const {
    int index = iPositions.indexOf(number);

    if(index == INVALID) {
        //throw IllegalArgumentException(QString("Animation::getLedPosition : led %1 does not exist").arg(number));
        //return false;
        return Position();
    }

    return Position(index/numRows(), index%numRows());

   /* *row = index/numRows();
    *column = index%numRows();

    return true;*/
}

int Animation::gridPositionNumber(int row, int column) const {
    return (row*numColumns()) + column;
}

void Animation::setGridPositionNumber(int row, int column, int number) {
    iPositions[gridPositionNumber(row, column)] = number;
}

void Animation::setClipboardGridPositionNumber(int row, int column, int number) {
    iClipboardPositions[gridPositionNumber(row, column)] = number;
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
    if(iRepeat) {
        setCurrentFrame((currentFrame()%numFrames()) + 1);
    } else {
        if(currentFrame() == numFrames()) {
            stop();
        } else {
            setCurrentFrame(currentFrame() + 1);
        }
    }

}

void Animation::copyToClipboard() {
    LedAnimStringCodec codec(*this);
    codec.writeAnimation(false);

    QClipboard *clipboard = QApplication::clipboard();
    QMimeData *data = new QMimeData;

    data->setData("text/plain", codec.asString().toUtf8());
    clipboard->setMimeData(data);
}
