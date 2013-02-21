/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include "ledanimcodec.h"

#include "led.h"
#include "frame.h"
#include "animation.h"

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

void LedAnimCodec::writeAnimation() {
    writeControlCharacter(HEADER_BYTE);

    writeCharacter(iAnimation.numRows());
    writeCharacter(iAnimation.numColumns());
    writeCharacter(iAnimation.numLeds());
    writeCharacter(iAnimation.numFrames());
    writeCharacter(iAnimation.frameFrequency());

    writePositionData();
    writeColourData();

    writeControlCharacter(TERMINATING_BYTE);
}

void LedAnimCodec::readAnimation() {
    if (readCharacter().charValue() != HEADER_BYTE) {
        throw new InvalidAnimationException("No header byte");
    }

    int numRows = readCharacter().intValue();
    int numColumns = readCharacter().intValue();
    int numLeds = readCharacter().intValue();
    int numFrames = readCharacter().intValue();
    int frameFrequency = readCharacter().intValue();

    iAnimation.setupNew(numRows,
                        numColumns,
                        numFrames,
                        frameFrequency,
                        numLeds,
                        readPositionData(numRows, numColumns, numLeds));

    readColourData();

    if (readCharacter().charValue() != TERMINATING_BYTE) {
        throw new InvalidAnimationException("No terminating byte");
    }
}

void LedAnimCodec::writeColour(QColor colour) {
    writeCharacter(colour.red());
    writeCharacter(colour.green());
    writeCharacter(colour.blue());
}

const QColor LedAnimCodec::readColour() const {
    int red = readCharacter().intValue();
    int green = readCharacter().intValue();
    int blue = readCharacter().intValue();

    return QColor::fromRgb(red, green, blue);
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

   /* if(value == TERMINATING_BYTE ||
       value == ESCAPE_BYTE ||
       value == HEADER_BYTE ) {
        iString.append(QString::number(ESCAPE_BYTE));
        iString.append(",");
        iString.append(QString::number((value ^ XOR_BYTE)));
    } else {*/
        iString.append(QString::number(value));
   // }

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
    for (int frame = 0; frame < iAnimation.numFrames(); frame++) {
        for(int i = 0; i < iAnimation.numLeds(); i++) {
            writeColour(iAnimation.ledAt(i).frameAt(frame + INITIAL_FRAME).colour());
        }

    }
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

void LedAnimByteArrayCodec::writeAnimation() {
    LedAnimCodec::writeAnimation();

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
   /* if(iByteArray.at(iPosition) == ESCAPE_BYTE) {
        AnimChar character(iByteArray.at(iPosition+1) ^ XOR_BYTE);
        iPosition = iPosition + 2;
        return character;
    } else {*/
        return AnimChar(iByteArray.at(iPosition++));
    //}
}

void LedAnimByteArrayCodec::writeCharacter(AnimChar character) {
    char value = character.charValue();

    /*if(value == TERMINATING_BYTE ||
       value == ESCAPE_BYTE ||
       value == HEADER_BYTE ) {
        iByteArray.append(ESCAPE_BYTE);
        iByteArray.append(value ^ XOR_BYTE);
    } else {*/
        iByteArray.append(value);
   // }
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
    int row;
    int column;

    for(int i = 0; i < iAnimation.numLeds(); i++) {
        iAnimation.getLedPosition(i, &row, &column);

        writeCharacter(AnimChar(row));
        writeCharacter(AnimChar(column));
    }
}

QList<int> LedAnimByteArrayCodec::readPositionData(int numRows, int numColumns, int numLeds) {
    QList<int> positions;

    for(int i = 0; i < numRows * numColumns; i++) {
        positions.append(INVALID);
    }

    int row;
    int column;

    for(int i = 0; i < numLeds; i++) {
        row = readCharacter().intValue();
        column = readCharacter().intValue();

        positions.replace((row*numColumns) + column, i);
    }

    return positions;
}

void LedAnimByteArrayCodec::writeColourData() {
    try {
        for (int frame = 0; frame < iAnimation.numFrames(); frame++) {
            for(int i = 0; i < iAnimation.numLeds(); i++) {
                writeColour(iAnimation.ledAt(i + INITIAL_LED).frameAt(frame + INITIAL_FRAME).colour());
            }
        }
    } catch (InvalidStateException* e) {
        // WTF?
    }
}

void LedAnimByteArrayCodec::readColourData() {
    for(int frame = 0; frame < iAnimation.numFrames(); frame++) {
        for(int i = 0; i < iAnimation.numLeds(); i++) {
            iAnimation.ledAt(i + INITIAL_LED).frameAt(frame + INITIAL_FRAME).setColour(readColour());
        }
    }
}

const QString LedAnimByteArrayCodec::asString() const {
    return "";
}

const QByteArray &LedAnimByteArrayCodec::asByteArray() const {
    return iByteArray;
}

