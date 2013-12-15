#include "LedSet.h"

#include "exceptions.h"

using namespace AnimatorModel;
using namespace Exception;

LedSet::LedSet(QObject *parent) :
    QObject(parent),
    iIterator(NULL),
    iHighestNumber(INVALID),
    iNumRows(0),
    iNumColumns(0) {
}

LedSet::LedSet(const LedSet& set)  :
    QObject(set.parent()),
    iIterator(NULL),
    iHighestNumber(set.iHighestNumber),
    iNumRows(set.iNumRows),
    iNumColumns(set.iNumColumns){

}

LedSetIterator& LedSet::iterator() {
    if(iIterator != NULL) {
        delete iIterator;
    }

    iIterator = new LedSetIterator(this, *this);
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

    return Position(index/iNumColumns, index%iNumColumns);
}

void LedSet::setPositionNumber(Position position, int number) {
    iPositions[positionNumber(position)] = number;
}

int LedSet::positionNumber(Position position) const {
    return (position.row()*iNumColumns) + position.column();
}
