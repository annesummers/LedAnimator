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

    setNumRows(numRows);
    setNumColumns(numColumns);

    for(int i = 0; i < numRows; i++) {
        for(int j = 0; j < numColumns; j++) {
            emit newSocket(i, j);
        }
    }

    setSaved(false);
    setFileName("");

    if(ledPositions.count() != 0) {
        iLeds->setPositions(ledPositions);
    } else {
        iLeds->clearPositions();
    }

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
}

void Animation::newAnimation(int numRows,
                  int numColumns) {
    newAnimation(numRows, numColumns, 0, QList<int>() );
}

const void Animation::addTimeAxis(int lowValue,
                 int highValue,
                 int speed,
                  int priority,
                  bool isOpaque) {
    iTimeAxis = new TimeAxis(this,
                             *this,
                             lowValue,
                             highValue,
                             speed,
                             priority,
                             isOpaque);

    emit timeAxisAdded();
}

const int Animation::addValueAxis(int lowValue,
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

    return axisNumber;
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

    Led* led = new Led(this, *this, ledNum != INVALID ? ledNum : nextLedNumber(), position, *iUndoStack);
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
    Led& led = *ledAt(fromPosition);
    Led* toLed = ledAt(toPosition);

    if(toLed != NULL) {
        return;
        //deleteLed(toRow, toColumn);
    }

    iLeds->moveLed(led, fromPosition, toPosition);

    emit ledMoved(fromPosition.row(), fromPosition.column(), toPosition.row(), toPosition.column());
}

Led* Animation::doCloneLed(Position fromPosition, Position toPosition) {
    Led* oldLed = ledAt(toPosition);
    Led* newLed = NULL;
    if(oldLed == NULL) {
        newLed = new Led(this, *this, nextLedNumber(), toPosition, *iUndoStack);
        addLed(*newLed, toPosition);
    } else {
        newLed = oldLed;
        oldLed = new Led(*newLed);
    }

    newLed->copyAxes(*ledAt(fromPosition));

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

    Led* led = iClipboardLeds->findLed(fromPosition);
    if(led == NULL) {
        throw IllegalArgumentException("Animation::pasteLed : Led is NULL");
    }

    iClipboardLeds->removeLed(*led);

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
    iClipboardLeds->addLed(*led);
}

void Animation::deleteLedFromClipboard(int ledNumber) {
    Led* led = iClipboardLeds->findLed(ledNumber);
    if(led != NULL) {
        iClipboardLeds->removeLed(*led);
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
