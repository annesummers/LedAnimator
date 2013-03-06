/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

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
    iRedIncrement((startColour.redF() - endColour.redF()) / iIterations),
    iGreenIncrement((startColour.greenF() - endColour.greenF()) / iIterations),
    iBlueIncrement((startColour.blueF() - endColour.blueF()) / iIterations),
    iTimer(NULL){

   /* iCurrentHueValue = startColour.hueF();
    iCurrentSatValue = startColour.saturationF();
    iCurrentValValue = startColour.valueF();*/

    iCurrentRedValue = startColour.redF();
    iCurrentGreenValue = startColour.greenF();
    iCurrentBlueValue = startColour.blueF();

  /*  iHueIncrement = (startColour.hueF() - endColour.hueF()) / iIterations;
    iSatIncrement = (startColour.saturationF() - endColour.saturationF()) / iIterations;
    iValIncrement = (startColour.valueF() - endColour.valueF()) / iIterations;*/

    resetColour();
}

void FadeCalculator::start() {
    qDebug("start fade");
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

  /*  iCurrentHueValue -= iHueIncrement;
    iCurrentSatValue -= iSatIncrement;
    iCurrentValValue -= iValIncrement;*/
    qDebug("red : %f green : %f blue %f", iCurrentRedValue, iCurrentGreenValue, iCurrentBlueValue);

    if(iCurrentRedValue > 1) {
        iCurrentRedValue = 1;
    }

    if(iCurrentBlueValue > 1) {
        iCurrentBlueValue = 1;
    }

    if(iCurrentGreenValue > 1) {
        iCurrentGreenValue = 1;
    }

    if(iCurrentRedValue < 0) {
        iCurrentRedValue = 0;
    }

    if(iCurrentBlueValue < 0) {
        iCurrentBlueValue = 0;
    }

    if(iCurrentGreenValue < 0) {
        iCurrentGreenValue = 0;
    }

    QColor colour = currentColour();
    if(!colour.isValid()) {
        qDebug("Colour is not valid");
    }

    emit colourCalculated(colour);

    if(++iCurrentIteration == iIterations) {
        if(currentColour() != iEndColour) {
            qDebug("End colour is %d, %d, %d Actual is %d, %d, %d",
                   iEndColour.hue(),
                   iEndColour.saturation(),
                   iEndColour.value(),
                   currentColour().hue(),
                   currentColour().saturation(),
                   currentColour().value());
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
   return QColor::fromRgbF(iCurrentRedValue, iCurrentGreenValue, iCurrentBlueValue);
}

void FadeCalculator::resetColour() {
    iCurrentRedValue = iStartColour.redF();
    iCurrentGreenValue = iStartColour.greenF();
    iCurrentBlueValue = iStartColour.blueF();
}
