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
#include "ledanimatoractionbase.h"

#include "constants.h"
#include "exceptions.h"

#include <QtDebug>

using namespace Exception;
using namespace ImportExport;
using namespace AnimatorModel;

Animation::Animation(Engine& engine) :
    QObject(&engine),
    iEngine(engine),
    iUndoStack(NULL),
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

    LedSetIterator& iterator = iLeds.iterator();

    while(iterator.hasNext()) {
        Led& led = iterator.next();
        delete &led;
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
        iLeds.setPositions(ledPositions);
    } else {
        iLeds.clearPositions();
    }

    int ledNum = INITIAL_LED;

    iLeds.clearMissing();
    for(int i = 0; i < numLeds; i++) {
        Position position;
        do {
            position = iLeds.ledPosition(ledNum);
            if(!position.isValid()) {
                iLeds.addMissing(ledNum++);
            }
        } while(!position.isValid());

        doAddNewLed(position, ledNum++);
    }

    iLeds.setHighestNumber(ledNum - 1);

    const QClipboard *clipboard = QApplication::clipboard();

    if(clipboard->mimeData()->hasFormat(LED_MIME_TYPE) ||
       clipboard->mimeData()->hasFormat(FRAME_MIME_TYPE)) {
        QApplication::clipboard()->clear();
    }

    iClipboardLeds.clearAll();
}

void Animation::setupNew(int numRows, int numColumns, int numFrames, int frameFrequency) {
    setupNew(numRows, numColumns, numFrames, frameFrequency, 0, QList<int>());
}

void Animation::addNewLed(Position position) {
    if(iUndoStack == NULL) {
        doAddNewLed(position);
    } else {
        iUndoStack->push(new AddLedCommand(*this, position));
    }
}

void Animation::doAddNewLed(Position position, int ledNum) {
    if(position.row() < 0) {
        throw IllegalArgumentException("Animation::addNewLed : row is negative");
    }

    if(position.column() < 0) {
        throw IllegalArgumentException("Animation::addNewLed : column is negative");
    }

    if(position.row() >= numRows()) {
        throw IllegalArgumentException("Animation::addNewLed : row is bigger than num rows");
    }

    if(position.column() >= numColumns()) {
        throw IllegalArgumentException("Animation::addNewLed : column is bigger than num columns");
    }

    Led* led = new Led(this, *this, ledNum != INVALID ? ledNum : nextLedNumber(), position);
    addLed(*led, position);

}

void Animation::addLed(Led& led, Position position) {
    iLeds.addLed(led);
    led.numFramesChanged(iNumFrames);

    emit newLed(position.row(), position.column());
}

void Animation::deleteLed(Led& led, bool deleteObject) {
    if(iUndoStack == NULL) {
        doDeleteLed(led.position(), deleteObject);
    } else {
        iUndoStack->push(new DeleteLedCommand(*this, led, deleteObject));
    }
}

void Animation::doDeleteLed(Position position, bool deleteObject) {
    Led* led = ledAt(position);
    int number = led->number();

    iLeds.removeLed(*led);

    emit ledDeleted(position.row(), position.column(), number);

    if(deleteObject) {
        delete led;
    }
    // TODO why does this fail?

}

void Animation::moveLed(Position fromPosition, Position toPosition) {
  //  iUndoStack->push(new MoveLedCommand(*this, fromPosition, toPosition));
    doMoveLed(fromPosition, toPosition);
}

void Animation::doMoveLed(Position fromPosition, Position toPosition) {
    Led& led = *ledAt(fromPosition);
    Led* toLed = ledAt(toPosition);

    if(toLed != NULL) {
        return;
        //deleteLed(toRow, toColumn);
    }

    iLeds.moveLed(led, fromPosition, toPosition);

    emit ledMoved(fromPosition.row(), fromPosition.column(), toPosition.row(), toPosition.column());
}

Led* Animation::doCloneLed(Position fromPosition, Position toPosition) {
    Led* oldLed = ledAt(toPosition);
    Led* newLed = NULL;
    if(oldLed == NULL) {
        newLed = new Led(this, *this, nextLedNumber(), toPosition);
        addLed(*newLed, toPosition);
    } else {
        newLed = oldLed;
        oldLed = new Led(this, *this, oldLed->number(), oldLed->position());
        oldLed->numFramesChanged(numFrames());
        oldLed->copyFrames(*newLed);
    }

    newLed->copyFrames(*ledAt(fromPosition));

    return oldLed;
}

void Animation::doPasteLed(Position fromPosition, Position toPosition, Led **fromLed, Led **toLed) {
    if(toPosition.row() >= numRows()) {
        throw IllegalArgumentException("Animation::pasteLed : Row is greater than number of rows");
    }

    if(toPosition.row() < 0) {
        throw IllegalArgumentException("Animation::pasteLed : Row is negative");
    }

    if(toPosition.column() >= numColumns()) {
        throw IllegalArgumentException("Animation::pasteLed : Column is greater than number of columns");
     }

    if(toPosition.column() < 0 ) {
        throw IllegalArgumentException("Animation::pasteLed : Column is negative");
    }

    *toLed = ledAt(toPosition);

    Led* led = iClipboardLeds.findLed(fromPosition);
    if(led == NULL) {
        throw IllegalArgumentException("Animation::pasteLed : Led is NULL");
    }

    iClipboardLeds.removeLed(*led);

    led->move(toPosition);
    addLed(*led, toPosition);

    *fromLed = led;
}

void Animation::cloneLed(Position fromPosition, Position toPosition) {
    if(iUndoStack == NULL) {
        doCloneLed(fromPosition, toPosition);
    } else {
        iUndoStack->push(new CloneLedCommand(*this, fromPosition, toPosition));
    }
}

void Animation::moveLedToClipboard(Position position) {
    addLedToClipboard(ledAt(position));

    deleteLed(*ledAt(position), false);
}

void Animation::addLedToClipboard(Led* led) {
    iClipboardLeds.addLed(*led);
}

void Animation::deleteLedFromClipboard(int ledNumber) {
    Led* led = iClipboardLeds.findLed(ledNumber);
    if(led != NULL) {
        iClipboardLeds.removeLed(*led);
        delete led;
    }
}

void Animation::pasteLed(Position fromPosition, Position toPosition) {
    if(iUndoStack == NULL) {
        Led* fromLed;
        Led* toLed;
        doPasteLed(fromPosition, toPosition, &fromLed, &toLed);
    } else {
        iUndoStack->push(new PasteLedCommand(*this, fromPosition, toPosition));
    }
}

void Animation::setFrameColour(Frame& frame, QColor oldColour, QColor newColour) {
    if(iUndoStack == NULL) {
        frame.doSetColour(newColour);
    } else {
        iUndoStack->push(new SetFrameColourCommand(*this, frame, oldColour, newColour));
    }
}

void Animation::renumberLed(Position position, int oldNumber, int newNumber) {
    if(iUndoStack == NULL) {
        doRenumberLed(position, newNumber);
    } else {
        iUndoStack->push(new RenumberLedCommand(*this, position, oldNumber, newNumber));
    }
}

void Animation::doRenumberLed(Position position, int newNumber) {
    Led& led = *ledAt(position);

    int oldNumber = led.number();

    iLeds.removeLed(led);
    led.setNumber(newNumber);
    iLeds.addLed(led);

    emit ledRenumbered(position.row(), position.column(), oldNumber);
}

int Animation::nextLedNumber() const {
    return iLeds.highestNumber()  + 1;
}

void Animation::selectGroup(int groupNumber) {
    LedSetIterator& iterator = iLeds.iterator();

    while(iterator.hasNext()) {
        Led& led = iterator.next();
        if(led.groupNumber() == groupNumber) {
            led.select(true);
        } else {
            led.select(false);
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

Led *Animation::ledAt(Position position) const {
    return iLeds.findLed(position);
}

Led* Animation::ledAt(int number) const {
    return iLeds.findLed(number);
}

Position Animation::ledPosition(int number) const {
    return iLeds.ledPosition(number);
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

LedSet::LedSet() :
    iIterator(NULL),
    iHighestNumber(INVALID),
    iNumRows(0),
    iNumColumns(0) {
}

LedSet::LedSet(const LedSet& set)  :
    iIterator(NULL),
    iHighestNumber(set.iHighestNumber),
    iNumRows(set.iNumRows),
    iNumColumns(set.iNumColumns){

}

LedSetIterator& LedSet::iterator() {
    if(iIterator != NULL) {
        delete iIterator;
    }
    iIterator = new LedSetIterator(this);
    return *iIterator;
}

void LedSet::addLed(Led &led)  {
    int ledNumber = led.number();

    if(ledNumber > iHighestNumber) {
        iHighestNumber = ledNumber;
    }

    if(isMissing(ledNumber)) {
        iFreeNumbers.removeOne(ledNumber);
    }

    if(iLeds.contains(ledNumber)) {
        delete iLeds.value(ledNumber);
    }

    iLeds.insert(ledNumber, &led);
    setPositionNumber(led.position(), ledNumber);
}

void LedSet::removeLed(Led& led) {
    iLeds.remove(led.number());

    if(led.number() == iHighestNumber) {
        iHighestNumber--;
    } else {
        iFreeNumbers.append(led.number());
    }

    setPositionNumber(led.position(), INVALID);
}

Led* LedSet::findLed(Position position) const {
    if(position.row() >= iNumRows) {
        throw IllegalArgumentException("Animation::ledAt : Row is greater than number of rows");
    }

    if(position.row() < 0) {
        throw IllegalArgumentException("Animation::ledAt : Row is negative");
    }

    if(position.column() >= iNumColumns) {
        throw IllegalArgumentException("Animation::ledAt : Column is greater than number of columns");
     }

    if(position.column() < 0 ) {
        throw IllegalArgumentException("Animation::ledAt : Column is negative");
    }

    return findLed(iPositions.at(positionNumber(position)));
}

Led* LedSet::findLed(int ledNumber) const  {

    if(iLeds.contains(ledNumber)) {
        return iLeds.value(ledNumber);
    } else {
       // throw IllegalArgumentException(QString("Animation::ledAt : led %1 does not exist").arg(number));
        return NULL;
    }
}

void LedSet::setPositions(QList<int> positions) {
    iPositions = positions;
}

void LedSet::clearPositions() {
    iPositions.clear();
    for(int i = 0; i < iNumRows * iNumColumns; i++) {
        iPositions.append(INVALID);
    }
}

void LedSet::moveLed(Led& led, Position fromPosition, Position toPosition) {
    setPositionNumber(fromPosition, INVALID);
    setPositionNumber(toPosition, led.number());

    led.move(toPosition);
}

void LedSet::clearAll() {
    clear();
    clearPositions();
}

void LedSet::clearMissing() {
    iFreeNumbers.clear();
}

void LedSet::addMissing(int missingLed) {
    iFreeNumbers.append(missingLed);
}

Position LedSet::ledPosition(int ledNumber) const {
    int index = iPositions.indexOf(ledNumber);

    if(index == INVALID) {
        //throw IllegalArgumentException(QString("Animation::getLedPosition : led %1 does not exist").arg(number));
        //return false;
        return Position();
    }

    return Position(index/iNumRows, index%iNumRows);
}

void LedSet::setPositionNumber(Position position, int number) {
    iPositions[positionNumber(position)] = number;
}

int LedSet::positionNumber(Position position) const {
    return (position.row()*iNumColumns) + position.column();
}

LedSetIterator::LedSetIterator (const LedSet* set) :
    QObject(NULL),
    iLedSet(*set),
    iNextCounter(0),
    iPreviousCounter(-1) {

}

bool LedSetIterator::findNext (const int group) {
    while(hasNext()) {
        if(next().groupNumber() == group) {
            return true;
        }
    }

    return false;
}

bool LedSetIterator::findPrevious (const int group) {
    while(hasPrevious()) {
        if(previous().groupNumber() == group) {
            return true;
        }
    }

    return false;
}

bool LedSetIterator::hasNext () const {
    if(iNextCounter > iLedSet.iHighestNumber ||
       iNextCounter  > iLedSet.count()) {
        return false;
    }

    return true;
}

bool LedSetIterator::hasPrevious () const {
    if(iPreviousCounter < 0) {
        return false;
    }

    return true;
}

Led& LedSetIterator::next () {
    while(!iLedSet.iLeds.contains(iNextCounter)  && iNextCounter < iLedSet.iHighestNumber) {
        iNextCounter++;
    }

    if(iNextCounter > iLedSet.iHighestNumber) {
        throw IllegalStateException("LedSetIterator::next() : no next item");
    }

    iPreviousCounter = iNextCounter;
    return *iLedSet.iLeds.value(iNextCounter++);
}

const Led& LedSetIterator::peekNext () const {
    int counter = iNextCounter;
    while(!iLedSet.iLeds.contains(counter) && counter < iLedSet.iHighestNumber) {
        counter++;
    }

    return *iLedSet.iLeds.value(counter);
}

const Led& LedSetIterator::peekPrevious () const {
    int counter = iPreviousCounter;
    while(!iLedSet.iLeds.contains(counter) && iPreviousCounter > INVALID) {
        counter--;
    }

    return *iLedSet.iLeds.value(counter);
}

Led& LedSetIterator::previous () {
    while(!iLedSet.iLeds.contains(iPreviousCounter && iPreviousCounter > INVALID)) {
        iPreviousCounter--;
    }

    if(iPreviousCounter < 0) {
        throw IllegalStateException("LedSetIterator::previous() : no previous item");
    }

    iNextCounter = iPreviousCounter;
    return *iLedSet.iLeds.value(iPreviousCounter--);
}

void LedSetIterator::toBack() {
    iNextCounter = INVALID;
    iPreviousCounter = iLedSet.iHighestNumber;
}

void LedSetIterator::toFront() {
    iNextCounter = 0;
    iPreviousCounter = INVALID;
}

/*void Animation::addLed(Led& led, int number) {
  //  iLeds.insert(number, &led);
    iLeds.addLed(&led);

    if(number > iGreatestNumber) {
        iGreatestNumber = number;
    }

    if(isMissing(number)) {
        iMissingLeds.removeOne(number);
    }

    setGridPositionNumber(led.position(), led.number());
}*/

/*void Animation::removeLed(Led& led) {
    iLeds.removeLed(led.number());

    if(led.number() == iGreatestNumber) {
        iGreatestNumber--;
    } else {
        iMissingLeds.append(led.number());
    }

    setGridPositionNumber(led.position(), INVALID);
}
*/
