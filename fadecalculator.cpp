#include "fadecalculator.h"

#include "exceptions.h"

using namespace Exception;

// TODO not a very accurate fade

FadeCalculator::FadeCalculator(QObject *parent,
                               const QColor startColour,
                               const QColor endColour,
                               const int iterations) :
    QObject(parent),
    iStartColour(startColour),
    iEndColour(endColour),
    iIterations(iterations),
    iRedIncrement((startColour.red() - endColour.red()) / iterations),
    iGreenIncrement((startColour.green() - endColour.green()) / iterations),
    iBlueIncrement((startColour.blue() - endColour.blue()) / iterations),
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
    iCurrentRedValue -= iRedIncrement;
    iCurrentGreenValue -= iGreenIncrement;
    iCurrentBlueValue -= iBlueIncrement;

    emit colourCalculated(currentColour());

    if(++iCurrentIteration == iIterations) {
        if(currentColour() != iEndColour) {
        //    throw new IllegalColourException();
        }

        if(iTimer != NULL) {
            iTimer->stop();
            delete iTimer;
            iTimer = NULL;
        }

        emit fadeComplete();
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
