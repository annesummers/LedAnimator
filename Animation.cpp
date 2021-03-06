/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include "Animation.h"

#include "Led.h"
#include "mainwindow.h"
#include "ledwidget.h"
#include "StringCodec.h"
#include "ledanimatoractionbase.h"
#include "TimeAxis.h"
#include "ValueAxis.h"

#include "constants.h"
#include "exceptions.h"

#include <QtDebug>

using namespace Exception;
using namespace ImportExport;
using namespace AnimatorModel;
using namespace UndoCommands;

Animation::Animation(Engine& engine) :
    QObject(&engine),
    iEngine(engine),
    iUndoStack(NULL),
    iLeds(NULL),
    iClipboardLeds(NULL),
    iNumRows(INVALID),
    iNumColumns(INVALID),
    iTimeAxis(NULL),
    iFileName(""),
    iIsSaved(false){ }

void Animation::newAnimation(int numRows,
                  int numColumns,
                  int numLeds,
                  QList<int> ledPositions) {
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

    if(iLeds != NULL) {
        LedSetIterator& iterator = iLeds->iterator();

        while(iterator.hasNext()) {
            Led& led = iterator.next();
            delete &led;
        }

        iLeds->clear();
    } else {
        iLeds = new LedSet(this);
    }

    const QClipboard *clipboard = QApplication::clipboard();

    if(clipboard->mimeData()->hasFormat(LED_MIME_TYPE) ||
       clipboard->mimeData()->hasFormat(FRAME_MIME_TYPE)) {
        QApplication::clipboard()->clear();
    }

    if(iClipboardLeds != NULL) {
        iClipboardLeds->clearAll();
    } else {
        iClipboardLeds = new LedSet(this);
    }

    iFunctions.clear();
    iFunctions.append(Function());

    if(iTimeAxis != NULL) {
        delete iTimeAxis;
        iTimeAxis = NULL;
    }

    Axis* axis;
    foreach(axis, iAxes) {
        delete axis;
    }

    iAxes.clear();

    setNumRows(numRows);
    setNumColumns(numColumns);

    for(int i = 0; i < numRows; i++) {
        for(int j = 0; j < numColumns; j++) {
            emit newSocketAdded(i, j);
        }
    }

    setSaved(false);
    setFileName("");

    if(ledPositions.count() != 0) {
        iLeds->setPositions(ledPositions);
    } else {
        iLeds->clearPositions();
    }

    iClipboardLeds->clearPositions();

    int ledNum = INITIAL_LED;

    iLeds->clearMissing();
    for(int i = 0; i < numLeds; i++) {
        Position position;
        do {
            position = iLeds->ledPosition(ledNum);
            if(!position.isValid()) {
                iLeds->addMissing(ledNum++);
            }
        } while(!position.isValid());

        doAddNewLed(position, ledNum++);
    }

    iLeds->setHighestNumber(ledNum - 1);
}

void Animation::newAnimation(int numRows,
                  int numColumns) {
    newAnimation(numRows, numColumns, 0, QList<int>() );
}

void Animation::addTimeAxis(int lowValue,
                            int highValue,
                            int frameRate,
                            int priority,
                            bool isOpaque) {

    iTimeAxis = new TimeAxis(this,
                             *this,
                             lowValue,
                             highValue,
                             frameRate,
                             priority,
                             isOpaque);

    emit timeAxisAdded();
}

void Animation::addValueAxis(int lowValue,
                            int highValue,
                            int zeroValue,
                            int priority,
                            bool isOpaque) {
    int axisNumber = iAxes.count();

    ValueAxis* axis = new ValueAxis(this,
                                    *this,
                                    axisNumber,
                                    lowValue,
                                    highValue,
                                    zeroValue,
                                    priority,
                                    isOpaque);

    iAxes.append(axis);

    emit valueAxisAdded(axisNumber);
}

void Animation::deleteValueAxis(int number) {
    ValueAxis* axis = iAxes.at(number);

    iAxes.removeAt(number);

    emit valueAxisDeleted(number);

    delete axis;
}

ValueAxis &Animation::axisAt(int number) const {
    return *(iAxes.at(number));
}

int Animation::addFunction(Function function) {
    for(int i = 0; i < iFunctions.size(); i++) {
        if(function == iFunctions.at(i)) {
            return i;
        }
    }

    iFunctions.append(function);
    return iFunctions.size() - 1;
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

    int ledNumber = ledNum != INVALID ? ledNum : nextLedNumber();

    if(ledNumber > numColumns() * numRows()) {
        throw IllegalArgumentException("Animation::addNewLed : num Leds is bigger than num sockets");
    }

    Led* led = new Led(this,
                       *this,
                       ledNumber,
                       position,
                       iUndoStack);

    addLed(*led, position);
}

void Animation::addLed(Led& led, Position position) {
    iLeds->addLed(led);

    if(iTimeAxis != NULL) {
        led.addTimeAxis();
    }

    for(int i = 0; i < iAxes.size(); i++) {
        led.addValueAxis(i);
    }

    emit newLedAdded(position.row(), position.column());
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

    if(led == NULL) {
        throw InvalidPositionException("Animation::deleteLed : led does not exist");
    }

    int number = led->number();

    iLeds->removeLed(*led);

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
    Led* fromLed = ledAt(fromPosition);

    if(fromLed == NULL) {
        throw InvalidPositionException("Animation::moveLed : led does not exist");
    }

    Led* toLed = ledAt(toPosition);

    if(toLed != NULL) {
        throw InvalidPositionException("Animation::moveLed : led already in this position");
        //deleteLed(toRow, toColumn);
    }

    iLeds->moveLed(*fromLed, fromPosition, toPosition);

    emit ledMoved(fromPosition.row(), fromPosition.column(), toPosition.row(), toPosition.column());
}

void Animation::cloneLed(Position fromPosition, Position toPosition) {
    if(iUndoStack == NULL) {
        doCloneLed(fromPosition, toPosition);
    } else {
        iUndoStack->push(new CloneLedCommand(*this, fromPosition, toPosition));
    }
}

Led* Animation::doCloneLed(Position fromPosition, Position toPosition) {
    Led* fromLed = ledAt(fromPosition);

    if(fromLed == NULL) {
        throw InvalidPositionException("Animation::cloneLed : led does not exist");
    }

    Led* cloneLed = ledAt(toPosition);
    Led* oldLed = NULL;

    if(cloneLed == NULL) {
        cloneLed = new Led(this,
                         *this,
                         nextLedNumber(),
                         toPosition,
                         iUndoStack);
        addLed(*cloneLed, toPosition);
    } else {
        oldLed = new Led(*cloneLed);
    }

    cloneLed->copyAxes(*fromLed);

    return oldLed;
}

void Animation::pasteClipboardLed(Position fromPosition, Position toPosition) {
    if(iUndoStack == NULL) {
        Led* fromLed;
        Led* toLed;
        doPasteClipboardLed(fromPosition, toPosition, &fromLed, &toLed);
    } else {
        iUndoStack->push(new PasteLedCommand(*this, fromPosition, toPosition));
    }
}

void Animation::doPasteClipboardLed(Position fromPosition, Position toPosition, Led **fromLed, Led **toLed) {
    Led* to = ledAt(toPosition);
    Led* from = iClipboardLeds->findLed(fromPosition);

    if(from == NULL) {
        throw InvalidPositionException("Animation::pasteLed : cannot find the led on the clipboard");
    }

    iClipboardLeds->removeLed(*from);

    if(to != NULL) {
        iLeds->removeLed(*to);
    }

    from->move(toPosition);
    addLed(*from, toPosition);

    *fromLed = from;
    *toLed = to;
}

void Animation::moveLedToClipboard(Position position) {
    addLedToClipboard(ledAt(position));

    deleteLed(*ledAt(position), false);
}

void Animation::addLedToClipboard(Led* led) {
    iClipboardLeds->addLed(*led);
}

void Animation::deleteLedFromClipboard(int number) {
    Led* led = iClipboardLeds->findLed(number);
    if(led != NULL) {
        iClipboardLeds->removeLed(*led);
        delete led;
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

    iLeds->removeLed(led);
    led.setNumber(newNumber);
    iLeds->addLed(led);

    emit ledRenumbered(position.row(), position.column(), oldNumber);
}

int Animation::nextLedNumber() const {
    return iLeds->highestNumber()  + 1;
}

Led *Animation::ledAt(Position position) const {
    return iLeds->findLed(position);
}

Led* Animation::ledAt(int number) const {
    return iLeds->findLed(number);
}

Position Animation::ledPosition(int number) const {
    return iLeds->ledPosition(number);
}

// slots -------------------------

void Animation::copyToClipboard() {
    LedAnimStringCodec codec(*this);
    codec.writeAnimation(false);

    QClipboard *clipboard = QApplication::clipboard();
    QMimeData *data = new QMimeData;

    data->setData("text/plain", codec.asString().toUtf8());
    clipboard->setMimeData(data);
}
/*
void Animation::addLed(Led& led, int number) {
  //  iLeds->insert(number, &led);
    iLeds->addLed(&led);

    if(number > iGreatestNumber) {
        iGreatestNumber = number;
    }

    if(isMissing(number)) {
        iMissingLeds.removeOne(number);
    }

    setGridPositionNumber(led.position(), led.number());
}*/

/*void Animation::removeLed(Led& led) {
    iLeds->removeLed(led.number());

    if(led.number() == iGreatestNumber) {
        iGreatestNumber--;
    } else {
        iMissingLeds.append(led.number());
    }

    setGridPositionNumber(led.position(), INVALID);
}
*/
