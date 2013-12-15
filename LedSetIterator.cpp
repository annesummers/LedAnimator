#include "LedSetIterator.h"

#include "Led.h"
#include "LedSet.h"
#include "exceptions.h"

using namespace AnimatorModel;
using namespace Exception;

LedSetIterator::LedSetIterator (QObject *parent, const LedSet& set) :
    QObject(parent),
    iLedSet(set),
    iNextCounter(0),
    iPreviousCounter(-1) {

}

bool LedSetIterator::findNext (const int group) {
  /*  while(hasNext()) {
        if(next().groupNumber() == group) {
            return true;
        }
    }*/

    return false;
}

bool LedSetIterator::findPrevious (const int group) {
  /*  while(hasPrevious()) {
        if(previous().groupNumber() == group) {
            return true;
        }
    }*/

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
