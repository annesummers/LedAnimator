#include "fadecalculator.h"

#include "exceptions.h"

using namespace Exception;

// TODO not a very accurate fade

FadeCalculator::FadeCalculator(QObject *parent, QColor startColour, QColor endColour, int iterations, bool backwards) :
    QObject(parent),
    iStartColour(startColour),
    iEndColour(endColour),
    iIterations(iterations),
    iRedIncrement((startColour.red() - endColour.red()) / iterations),
    iGreenIncrement((startColour.green() - endColour.green()) / iterations),
    iBlueIncrement((startColour.blue() - endColour.blue()) / iterations),
    iBackwards(backwards),
    iTimer(NULL){

    resetColour();
}

void FadeCalculator::start() {
    if(iTimer != NULL) {
        delete iTimer;
        iTimer = NULL;
    }

    iTimer = new QTimer(this);
    connect(iTimer, SIGNAL(timeout()), this, SLOT(nextColour()));
    iCurrentIteration = 0;

    emit colourCalculated(iStartColour);

    iTimer->start(1);
}

void FadeCalculator::nextColour() {
    if(!iBackwards) {
        iCurrentRedValue -= iRedIncrement;
        iCurrentGreenValue -= iGreenIncrement;
        iCurrentBlueValue -= iBlueIncrement;
    } else {
        iCurrentRedValue += iRedIncrement;
        iCurrentGreenValue += iGreenIncrement;
        iCurrentBlueValue += iBlueIncrement;
    }

    emit colourCalculated(currentColour());

    if(iCurrentIteration++ >= iIterations) {
        if(currentColour() != iEndColour) {
        //    throw new IllegalColourException();
        }

        iTimer->stop();
        delete iTimer;
        iTimer = NULL;
    }
}

QColor FadeCalculator::currentColour() {
    return QColor::fromRgb(iCurrentRedValue, iCurrentGreenValue, iCurrentBlueValue);
}

void FadeCalculator::resetColour() {
    iCurrentRedValue = iStartColour.red();
    iCurrentGreenValue = iStartColour.green();
    iCurrentBlueValue = iStartColour.blue();
}
