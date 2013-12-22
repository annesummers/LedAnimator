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

const int AnimChar::intValue() const {
    return static_cast<int>(iValue);
}

const unsigned char AnimChar::charValue() const {
    return iValue;
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

    //int numSubAnimations = iAnimation.numSubAnimations();
    writeCharacter(0);

   // int offset = numSubAnimations;
    writeCharacter(iAnimation.timeAxis()->lowValue());
   /* for(int i = 1; i < numSubAnimations; i++) {
        offset = iAnimation.numFrames() * iAnimation.numLeds() * 4 + (numSubAnimations - i);
        writeCharacter(offset);
    }*/

    writeCharacter(iAnimation.timeAxis()->highValue());
    writeCharacter(iAnimation.timeAxis()->speed());

    writeColourData();

    writeControlCharacter(TERMINATING_BYTE);
}

void LedAnimCodec::readAnimation() {
    if (readCharacter().charValue() != HEADER_BYTE) {
        throw new InvalidAnimationException("No header byte");
    }

    char numLedsHigh = readCharacter().charValue();
    char numLedsLow = readCharacter().charValue();

    int numLeds =  numLedsHigh |= numLedsLow << 8;

    int numRows;
    int numColumns;
    QList<int> positions = readPositionData(&numRows, &numColumns, numLeds);
    /*int numSubAnimations = */readCharacter().intValue();
    int lowValue = readCharacter().intValue();

    //int lowValue = 0;///readCharacter().intValue();
    int highValue = readCharacter().intValue();
    int frameFrequency = readCharacter().intValue();

    iAnimation.newAnimation(numRows,
                        numColumns ,
                        numLeds,
                        positions);
    iAnimation.addTimeAxis(lowValue, highValue, frameFrequency);

    readColourData();

    if (readCharacter().charValue() != TERMINATING_BYTE) {
        throw new InvalidAnimationException("No terminating byte");
    }
}

void LedAnimCodec::writeColour(Frame &frame) {
    const ColourValue& colourValue = static_cast<const ColourValue&>(frame.value());

    int hue = colourValue.colour().hue();
    char hueHigh = hue;
    char hueLow = hue >> 8;
    writeCharacter(hueHigh);
    writeCharacter(hueLow);
    writeCharacter(colourValue.colour().saturation());
    writeCharacter(colourValue.colour().value());
}

const QColor LedAnimCodec::readColour() const {
    unsigned char hueHigh = readCharacter().charValue();
    char hueLow = readCharacter().charValue();

    int hue = hueHigh;
    hue |= hueLow << 8;

    int saturation = readCharacter().intValue();
    int value = readCharacter().intValue();

    QColor colour = QColor::fromHsv(hue, saturation, value);

    if(!colour.isValid()) {
        qDebug("invalid colour");
    }

    return colour;
}

void LedAnimCodec::writeFunction(Frame& frame) {
    const FunctionValue& value = static_cast<const FunctionValue&>(frame.value());

    int redIncrement = (int)(value.function().redIncrement() * sizeof(int));
    int greenIncrement = (int)(value.function().greenIncrement() * sizeof(int));
    int blueIncrement = (int)(value.function().blueIncrement() * sizeof(int));

    writeCharacter((char) redIncrement);
    writeCharacter((char) redIncrement >> 8);
    writeCharacter((char) greenIncrement);
    writeCharacter((char) greenIncrement >> 8);
    writeCharacter((char) blueIncrement);
    writeCharacter((char) blueIncrement >> 8);
}

const Function LedAnimCodec::readFunction() const {
    unsigned char redHigh = readCharacter().charValue();
    char redLow = readCharacter().charValue();

    int intRedIncrement = redHigh;
    intRedIncrement |= redLow << 8;
    float redIncrement = ((float)intRedIncrement)/sizeof(int);

    unsigned char greenHigh = readCharacter().charValue();
    char greenLow = readCharacter().charValue();

    int intGreenIncrement = greenHigh;
    intGreenIncrement |= greenLow << 8;
    float greenIncrement = ((float)intGreenIncrement)/sizeof(int);

    unsigned char blueHigh = readCharacter().charValue();
    char blueLow = readCharacter().charValue();

    int intBlueIncrement = blueHigh;
    intBlueIncrement |= blueLow << 8;
    float blueIncrement = ((float)intBlueIncrement)/sizeof(int);

    return Function(redIncrement,
                    blueIncrement,
                    greenIncrement);
}

// -----------------------------------------

LedAnimStringCodec::LedAnimStringCodec(Animation &animation) :
    LedAnimCodec(animation),
    iLastReturn(0) {
}

const AnimChar LedAnimStringCodec::readCharacter() const {
    return AnimChar();
}

void LedAnimStringCodec::writeCharacter(AnimChar character) {
    int value = character.intValue();

    if(value == TERMINATING_BYTE ||
       value == ESCAPE_BYTE ||
       value == HEADER_BYTE ) {
        iString.append(QString::number(ESCAPE_BYTE));
        iString.append(",");
        iString.append(QString::number((value ^ XOR_BYTE)));
    } else {
        iString.append(QString::number(value));
    }

    iString.append(",");

    int extra = iString.length() - iLastReturn;
    if(extra > 100) {
        iString.append("\n");
        iLastReturn = iString.length();
    }
}

void LedAnimStringCodec::writeControlCharacter(AnimChar character) {
    int value = character.charValue();

    if(value != TERMINATING_BYTE &&
       value != HEADER_BYTE ) {
        throw IllegalArgumentException("LedAnimStringCodec::writeControlCharacter : not a control character");
    }

    iString.append(QString::number(character.intValue()));

    if(value == HEADER_BYTE) {
        iString.append(",");
    }
}

void LedAnimStringCodec::writeColourData() {
    /*for (int frame = 0; frame < iAnimation.numFrames(); frame++) {
        int ledNum = INITIAL_LED;
        for(int i = 0; i < iAnimation.numLeds(); i++) {
            while(iAnimation.isMissing(ledNum)) {
                ledNum++;
            }

            if(iWriteLedNumber) {
                writeCharacter(ledNum);
            }

            writeColour(iAnimation.ledAt(ledNum++)->frameAt(frame + INITIAL_FRAME).colour());
        }
    }*/
}

void LedAnimStringCodec::readColourData() {

}

const QString LedAnimStringCodec::asString() const {
    return iString;
}

const QByteArray& LedAnimStringCodec::asByteArray() const {
    return QByteArray();
}

// -----------------------------------------

LedAnimByteArrayCodec::LedAnimByteArrayCodec(Animation &animation) :
    LedAnimCodec(animation),
    iPosition(0) {}


LedAnimByteArrayCodec::LedAnimByteArrayCodec(Animation& animation, QByteArray bytes) :
    LedAnimCodec(animation),
    iByteArray(bytes),
    iPosition(0) {
}

void LedAnimByteArrayCodec::writeAnimation(bool withPositions) {
    LedAnimCodec::writeAnimation(withPositions);

    iAnimation.setSaved(true);
}

void LedAnimByteArrayCodec::readAnimation() {
    LedAnimCodec::readAnimation();

    iAnimation.setSaved(true);
}

const AnimChar LedAnimByteArrayCodec::readControlCharacter() const {
    char value = iByteArray.at(iPosition++);

    if(value != TERMINATING_BYTE &&
       value != HEADER_BYTE ) {
        throw IllegalArgumentException("LedAnimByteArrayCodec::readControlCharacter : not a control character");
    }

    return AnimChar(value);
}

const AnimChar LedAnimByteArrayCodec::readCharacter() const {
    if(iByteArray.at(iPosition) == ESCAPE_BYTE) {
        AnimChar character(iByteArray.at(iPosition+1) ^ XOR_BYTE);
        iPosition = iPosition + 2;
        return character;
    } else {
        return AnimChar(iByteArray.at(iPosition++));
    }
}

void LedAnimByteArrayCodec::writeCharacter(AnimChar character) {
    char value = character.charValue();

    if(value == TERMINATING_BYTE ||
       value == ESCAPE_BYTE ||
       value == HEADER_BYTE ) {
        iByteArray.append(ESCAPE_BYTE);
        iByteArray.append(value ^ XOR_BYTE);
    } else {
        iByteArray.append(value);
    }
}

void LedAnimByteArrayCodec::writeControlCharacter(AnimChar character) {
    char value = character.charValue();
    if(value != TERMINATING_BYTE &&
       value != HEADER_BYTE ) {
        throw IllegalArgumentException("LedAnimByteArrayCodec::writeControlCharacter : not a control character");
    }

    iByteArray.append(value);
}

void LedAnimByteArrayCodec::writePositionData() {
    writeCharacter(iAnimation.numRows());
    writeCharacter(iAnimation.numColumns());

    int ledNum = INITIAL_LED;
    for(int i = 0; i < iAnimation.numLeds(); i++) {
        Position position;
        do {
            position = iAnimation.ledPosition(ledNum);
            if(!position.isValid()) {
                ledNum++;
            }
        } while(!position.isValid());

        writeCharacter(ledNum++);
        writeCharacter(position.row());
        writeCharacter(position.column());
    }
}

QList<int> LedAnimByteArrayCodec::readPositionData(int* numRows, int* numColumns, int numLeds) {
    QList<int> positions;

    int rows = readCharacter().intValue();
    int columns = readCharacter().intValue();

    for(int i = 0; i < rows * columns; i++) {
        positions.append(INVALID);
    }

    int ledNum;
    int row;
    int column;

    for(int i = 0; i < numLeds; i++) {
        ledNum = readCharacter().intValue();
        row = readCharacter().intValue();
        column = readCharacter().intValue();

        if(row != INVALID && column != INVALID) {
            positions.replace((row*columns) + column, ledNum);
        }
    }

    *numRows = rows;
    *numColumns = columns;

    return positions;
}

void LedAnimByteArrayCodec::writeColourData() {
    for (int frame = iAnimation.timeAxis()->lowValue();
         frame < iAnimation.timeAxis()->highValue();
         frame++) {
        int ledNum = INITIAL_LED;
        for(int i = 0; i < iAnimation.numLeds(); i++) {
            while(iAnimation.isMissing(ledNum)) {
                qDebug("led %d is missing", ledNum);
                ledNum++;
            }

            if(iWriteLedNumber) {
                writeCharacter(ledNum++);
            }

            writeCharacter(iAnimation.ledAt(ledNum)->timeAxis()->frameAt(frame).value().type());

            switch(iAnimation.ledAt(ledNum)->timeAxis()->frameAt(frame).value().type()) {
            case kColour:
                writeColour(iAnimation.ledAt(ledNum)->timeAxis()->frameAt(frame));
                break;
            case kFunction:
                writeFunction(iAnimation.ledAt(ledNum)->timeAxis()->frameAt(frame));
                break;
            }
        }
    }
}

void LedAnimByteArrayCodec::readColourData() {
    for(int frame = iAnimation.timeAxis()->lowValue();
        frame < iAnimation.timeAxis()->highValue();
        frame++) {
        int ledNum = INITIAL_LED;
        for(int i = 0; i < iAnimation.numLeds(); i++) {
            readCharacter();
            while(iAnimation.isMissing(ledNum)){
                ledNum++;
            }

            Led* led = iAnimation.ledAt(ledNum++);

            char frameType = readCharacter().charValue();

            switch(frameType) {
            case kColour:
                led->timeAxis()->frameAt(frame).doSetValue(*(new ColourValue(led, readColour())));
                break;
            case kFunction:
                led->timeAxis()->frameAt(frame).doSetValue(*(new FunctionValue(led, readFunction())));
                break;
            }


        }
    }
}

const QString LedAnimByteArrayCodec::asString() const {
    return "";
}

const QByteArray &LedAnimByteArrayCodec::asByteArray() const {
    return iByteArray;
}

