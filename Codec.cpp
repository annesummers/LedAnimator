/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#include "Codec.h"

#include "Led.h"
#include "Frame.h"
#include "Animation.h"
#include "ColourValue.h"
#include "FunctionValue.h"
#include "FunctionFrame.h"

#include "constants.h"
#include "exceptions.h"

using namespace ImportExport;
using namespace Exception;

AnimChar::AnimChar(const int value) :
    iValue(value) { }

AnimChar::AnimChar(const unsigned char value) :
    iValue(value) { }

AnimChar::AnimChar() :
    iValue(INVALID){

}

int AnimChar::intValue() const {
    return static_cast<int>(iValue);
}

unsigned int AnimChar::unsignedIntValue() const {
    return static_cast<unsigned int>(iValue);
}

bool AnimChar::boolValue() const {
    return static_cast<bool>(iValue);
}

unsigned char AnimChar::unsignedCharValue() const {
    return static_cast<unsigned char>(iValue);
}

LedAnimCodec::LedAnimCodec(Animation& animation) :
    QObject(&animation),
    iAnimation(animation){
}

void LedAnimCodec::writeAnimation(bool withPositions) {
    if(withPositions) {
        iWriteLedNumber = true;
    }

    writeControlCharacter(HEADER_BYTE);

    int numLeds = iAnimation.numLeds();
    char numLedsHigh = numLeds;
    char numLedsLow = numLeds >> 8;
    writeCharacter(numLedsHigh);
    writeCharacter(numLedsLow);

    if(withPositions) {
        writePositionData();
    }

    writeCharacter(iAnimation.numFunctions());
    for(int i = 0; i < iAnimation.numFunctions(); i++) {
        writeFunctionData(iAnimation.functionAt(i));
    }

    writeCharacter(iAnimation.numValueAxes());

    writeAxis(kTimeAxisNum);

    for(int i = 0; i < iAnimation.numValueAxes(); i++) {
        writeAxis(i);
    }


    writeAxisData(kTimeAxisNum);

    writeControlCharacter(TERMINATING_BYTE);
}

void LedAnimCodec::readAnimation() {
    if (readCharacter().unsignedCharValue() != HEADER_BYTE) {
        throw new InvalidAnimationException("No header byte");
    }

    unsigned char numLedsHigh = readCharacter().unsignedCharValue();
    unsigned char numLedsLow = readCharacter().unsignedCharValue();

    int numLeds =  numLedsHigh |= numLedsLow << 8;

    int numRows;
    int numColumns;
    QList<int> positions = readPositionData(&numRows, &numColumns, numLeds);

    iAnimation.newAnimation(numRows,
                        numColumns ,
                        numLeds,
                        positions);

    int numFunctions = readCharacter().intValue();
    for(int i = 0; i < numFunctions; i++) {
        readFunctionData();
    }

    int numAxes = readCharacter().intValue();

    readAxis(kTimeAxisNum);

    for(int i = 0; i < numAxes; i++) {
        readAxis(i);
    }

    readAxisData(kTimeAxisNum);

    if (readCharacter().unsignedCharValue() != TERMINATING_BYTE) {
        throw new InvalidAnimationException("No terminating byte");
    }
}


void LedAnimCodec::writeAxis(int axisNum) {
    Axis* axis = NULL;

    if(axisNum == kTimeAxisNum) {
        axis = iAnimation.timeAxis();

        writeCharacter(kTimeAxis);
        writeCharacter(axis->priority());
        writeCharacter(axis->isOpaque());

        writeCharacter(axis->lowValue());
        writeCharacter(axis->highValue());
        writeCharacter(iAnimation.timeAxis()->speed());
        writeCharacter(iAnimation.timeAxis()->usesBackgroundColour());
        if(iAnimation.timeAxis()->usesBackgroundColour()) {
            writeColour(iAnimation.timeAxis()->backgroundColour());
        }
    } else {
        axis = &iAnimation.axisAt(axisNum);

        writeCharacter(kValueAxis);
        writeCharacter(axis->priority());
        writeCharacter(axis->isOpaque());

        writeCharacter(axis->lowValue());
        writeCharacter(axis->highValue());
        writeCharacter(iAnimation.axisAt(axisNum).zeroValue());

        for (int frame = axis->lowValue(); frame <= axis->highValue(); frame++) {
            int ledNum = INITIAL_LED;
            for(int i = 0; i < iAnimation.numLeds(); i++) {
                while(iAnimation.isMissing(ledNum)) {
                    qDebug("led %d is missing", ledNum);
                    ledNum++;
                }

                if(iWriteLedNumber) {
                    writeCharacter(ledNum);
                }

                AxisData* axisData = NULL;

                if(axisNum == kTimeAxisNum) {
                    axisData = iAnimation.ledAt(ledNum)->timeAxis();
                } else {
                    axisData = &iAnimation.ledAt(ledNum)->axisAt(axisNum);
                }

                writeCharacter(axisData->frameAt(frame).value().type());

                switch(axisData->frameAt(frame).value().type()) {
                case kColour:
                    writeColour(axisData->frameAt(frame));
                    break;
                case kFunction:
                    writeFunction(axisData->frameAt(frame));
                    break;
                }

                ledNum++;
            }
        }
    }

}

void LedAnimCodec::writeAxisData(int axisNum) {
    Q_UNUSED(axisNum);

    Axis* axis = iAnimation.timeAxis();

    for (int frame = axis->lowValue(); frame <= axis->highValue(); frame++) {
        int ledNum = INITIAL_LED;
        for(int i = 0; i < iAnimation.numLeds(); i++) {
            while(iAnimation.isMissing(ledNum)) {
                qDebug("led %d is missing", ledNum);
                ledNum++;
            }

            if(iWriteLedNumber) {
                writeCharacter(ledNum);
            }

            AxisData* axisData = NULL;

          //  if(axisNum == kTimeAxisNum) {
                axisData = iAnimation.ledAt(ledNum)->timeAxis();
          //  } else {
                //axisData = &iAnimation.ledAt(ledNum)->axisAt(axisNum);
          //  }

            writeCharacter(axisData->frameAt(frame).value().type());

            switch(axisData->frameAt(frame).value().type()) {
            case kColour:
                writeColour(axisData->frameAt(frame));
                break;
            case kFunction:
                writeFunction(axisData->frameAt(frame));
                break;
            }

            ledNum++;
        }
    }
}

void LedAnimCodec::readAxis(int axisNum) {
    Axis* axis = NULL;

    int axisType = readCharacter().intValue();
    int priority = readCharacter().intValue();
    bool opaque = readCharacter().boolValue();

    if(axisNum == kTimeAxisNum) {
        int lowValue = readCharacter().intValue();
        int highValue = readCharacter().intValue();
        int speed = readCharacter().intValue();

        iAnimation.addTimeAxis(lowValue, highValue, speed, priority, opaque);

        if(readCharacter().boolValue()) {
            iAnimation.timeAxis()->setBackgroundColour(readColour());
        }
    } else {
        int lowValue = readCharacter().intValue();
        int highValue = readCharacter().intValue();
        int zeroValue = readCharacter().intValue();

        iAnimation.addValueAxis(lowValue, highValue, zeroValue, priority, opaque);
        axis = &iAnimation.axisAt(axisNum);

        for(int frame = axis->lowValue(); frame <= axis->highValue(); frame++) {
            int ledNum = INITIAL_LED;
            for(int i = 0; i < iAnimation.numLeds(); i++) {
                readCharacter();
                while(iAnimation.isMissing(ledNum)){
                    ledNum++;
                }

                Led* led = iAnimation.ledAt(ledNum++);

                AxisData* axisData = NULL;

                if(axisNum == kTimeAxisNum) {
                    axisData = led->timeAxis();
                } else {
                    axisData = &led->axisAt(axisNum);
                }

                char frameType = readCharacter().unsignedCharValue();

                switch(frameType) {
                case kColour:
                    axisData->frameAt(frame).doSetValue(*(new ColourValue(led, readColour())));
                    break;
                case kFunction:
                    Frame& newFrame = axisData->frameAt(frame);
                    newFrame.doSetValue(readFunction(newFrame));
                    break;
                }


            }
        }
    }
}

void LedAnimCodec::readAxisData(int axisNum) {
    Q_UNUSED(axisNum);

    Axis* axis = iAnimation.timeAxis();

    if(axis != NULL) {

    for(int frame = axis->lowValue(); frame <= axis->highValue(); frame++) {
        int ledNum = INITIAL_LED;
        for(int i = 0; i < iAnimation.numLeds(); i++) {
            readCharacter();
            while(iAnimation.isMissing(ledNum)){
                ledNum++;
            }

            Led* led = iAnimation.ledAt(ledNum++);

            AxisData* axisData = NULL;

            //if(axisNum == kTimeAxisNum) {
                axisData = led->timeAxis();
           // } else {
           //     axisData = &led->axisAt(axisNum);
           // }

            char frameType = readCharacter().unsignedCharValue();

            switch(frameType) {
            case kColour:
                axisData->frameAt(frame).doSetValue(*(new ColourValue(led, readColour())));
                break;
            case kFunction:
                Frame& newFrame = axisData->frameAt(frame);
                newFrame.doSetValue(readFunction(newFrame));
                break;
            }


        }
    }
    }
}

