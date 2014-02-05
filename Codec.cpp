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
        iVerbose = true;
    }

    writeHeader();

    writeInt(iAnimation.numLeds());

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

    writeTimeAxisData();

    writeControlCharacter(TERMINATING_BYTE);
}

void LedAnimCodec::writeInt(int integer) {
    unsigned char integerHigh = integer;
    unsigned char integerLow = integer >> 8;

    writeCharacter(integerHigh);
    writeCharacter(integerLow);
}

void LedAnimCodec::writeHeader() {
    QString header = "LAAN";

    writeString(header.toStdString().c_str());
    writeCharacter(VERSION_HIGH);
    writeCharacter(VERSION_LOW);
}

void LedAnimCodec::readHeader() {
/*
     if (readCharacter().unsignedCharValue() != HEADER_BYTE) {
         throw new InvalidAnimationException("No header byte");
     }
*/
    if(!isAnimatorFile()) {
        throw InvalidFileException("The file is not a valid Led Animator file");
    }

    unsigned char fileVersionHigh = readCharacter().unsignedCharValue();
    unsigned char fileVersionLow = readCharacter().unsignedCharValue();

   /* if(fileVersionHigh > VERSION_HIGH ||
       fileVersionHigh == VERSION_HIGH && fileVersionLow < VERSION_LOW) {
        throw InvalidFileException("The file version is too low");
    }

    if(fileVersionHigh < VERSION_HIGH ||
       fileVersionHigh == VERSION_HIGH && fileVersionLow > VERSION_LOW) {
        throw InvalidFileException("The file version is too low");
    }*/
}

void LedAnimCodec::readAnimation() {
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
        iAnimation.addFunction(readFunctionData());
    }

    int numAxes = readCharacter().intValue();

    readAxis(kTimeAxisNum);

    for(int i = 0; i < numAxes; i++) {
        readAxis(i);
    }

    readTimeAxisData();

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
        writeInt(iAnimation.timeAxis()->frameRate());
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

        writeValueAxisData(axisNum);
    }
}

void LedAnimCodec::writeValueAxisData(int axisNum) {
    int ledNum = INITIAL_LED;
    Axis* axis = &iAnimation.axisAt(axisNum);

    int totalRangeCount = 0;

    for(int i = 0; i < iAnimation.numLeds(); i++) {
        while(iAnimation.isMissing(ledNum)) {
            qDebug("led %d is missing", ledNum);
            ledNum++;
        }

        AxisData& axisData = iAnimation.ledAt(ledNum)->axisAt(axisNum);
        totalRangeCount += axisData.numRanges();
        writeCharacter(axisData.numRanges());

        ledNum++;
    }

    ledNum = INITIAL_LED;

    for(int i = 0; i < iAnimation.numLeds(); i++) {
        while(iAnimation.isMissing(ledNum)) {
            qDebug("led %d is missing", ledNum);
            ledNum++;
        }

        AxisData& axisData = iAnimation.ledAt(ledNum)->axisAt(axisNum);

        if(ledNum == 61) {
            qDebug() << 61;
        }

        //axisData.calculateRanges();
        writeRanges(axisData);

        ledNum++;
    }

    if(iVerbose) {
        for (int frame = axis->lowValue(); frame <= axis->highValue(); frame++) {
            ledNum = INITIAL_LED;
            for(int i = 0; i < iAnimation.numLeds(); i++) {
                while(iAnimation.isMissing(ledNum)) {
                    qDebug("led %d is missing", ledNum);
                    ledNum++;
                }

                if(iVerbose) {
                    writeInt(ledNum);
                }

                AxisData* axisData = &iAnimation.ledAt(ledNum)->axisAt(axisNum);

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

void LedAnimCodec::writeRanges(AxisData &axisData) {
    writeCharacter(axisData.numRanges());
    for(int i = 0; i < axisData.numRanges(); i++) {
        Range range = axisData.rangeAt(i);
        writeCharacter(range.lowValue());
        writeCharacter(range.highValue());
        writeCharacter(range.anchor());
        writeCharacter(range.function());
    }
}

void LedAnimCodec::readRanges(AxisData& axisData) {
    int numRanges = readCharacter().intValue();
    for(int i = 0; i < numRanges; i++) {
        axisData.setRange(readCharacter().intValue(),
                            readCharacter().intValue(),
                            readCharacter().intValue(),
                            iAnimation.functionAt(readCharacter().intValue()));
    }
}

void LedAnimCodec::writeTimeAxisData() {
    Axis* axis = iAnimation.timeAxis();

    for (int frame = axis->lowValue(); frame <= axis->highValue(); frame++) {
        int ledNum = INITIAL_LED;
        for(int i = 0; i < iAnimation.numLeds(); i++) {
            while(iAnimation.isMissing(ledNum)) {
                qDebug("led %d is missing", ledNum);
                ledNum++;
            }

            if(iVerbose) {
                writeInt(ledNum);
            }

            AxisData* axisData = iAnimation.ledAt(ledNum)->timeAxis();

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
    int axisType = readCharacter().intValue();
    int priority = readCharacter().intValue();
    bool opaque = readCharacter().boolValue();

    if(axisNum == kTimeAxisNum) {
        int lowValue = readCharacter().intValue();
        int highValue = readCharacter().intValue();

        unsigned char frameRateHigh = readCharacter().unsignedCharValue();
        unsigned char frameRateLow = readCharacter().unsignedCharValue();
        int frameRate =  frameRateHigh |= frameRateLow << 8;

        iAnimation.addTimeAxis(lowValue, highValue, frameRate, priority, opaque);

        if(readCharacter().boolValue()) {
            iAnimation.timeAxis()->setBackgroundColour(readColour());
        }
    } else {
        int lowValue = readCharacter().intValue();
        int highValue = readCharacter().intValue();
        int zeroValue = readCharacter().intValue();

        iAnimation.addValueAxis(lowValue, highValue, zeroValue, priority, opaque);

        readValueAxisData(axisNum);
    }
}

void LedAnimCodec::readValueAxisData(int axisNum) {
    Axis* axis = &iAnimation.axisAt(axisNum);
    int ledNum = INITIAL_LED;

    for(int i = 0; i < iAnimation.numLeds(); i++) {
        while(iAnimation.isMissing(ledNum)){
            ledNum++;
        }

        readCharacter();

        ledNum++;
    }

    ledNum = INITIAL_LED;
    for(int i = 0; i < iAnimation.numLeds(); i++) {
        while(iAnimation.isMissing(ledNum)){
            ledNum++;
        }

        ValueAxisData& axisData = iAnimation.ledAt(ledNum)->axisAt(axisNum);

        readRanges(axisData);

        ledNum++;
    }

    for(int frame = axis->lowValue(); frame <= axis->highValue(); frame++) {
        ledNum = INITIAL_LED;
        for(int i = 0; i < iAnimation.numLeds(); i++) {
            readCharacter();
            readCharacter();
            while(iAnimation.isMissing(ledNum)){
                ledNum++;
            }

            Led* led = iAnimation.ledAt(ledNum);

            AxisData* axisData = &led->axisAt(axisNum);

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

            ledNum++;

        }
    }
}

void LedAnimCodec::readTimeAxisData() {
    Axis* axis = iAnimation.timeAxis();

    if(axis != NULL) {

    for(int frame = axis->lowValue(); frame <= axis->highValue(); frame++) {
        int ledNum = INITIAL_LED;
        for(int i = 0; i < iAnimation.numLeds(); i++) {
            readCharacter();
            readCharacter();
            while(iAnimation.isMissing(ledNum)){
                ledNum++;
            }

            Led* led = iAnimation.ledAt(ledNum);

            AxisData* axisData = led->timeAxis();
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

            ledNum++;
        }
    }
    }
}

