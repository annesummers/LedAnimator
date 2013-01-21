/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include "ledanimcodec.h"

#include "led.h"
#include "frame.h"
#include "animation.h"

#include "defaults.h"
#include "exceptions.h"

using namespace ImportExport;
using namespace Exception;

AnimChar::AnimChar(const int value) :
    iValue(value) { }

AnimChar::AnimChar(const unsigned char value) :
    iValue(value) { }

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
    //add frame frequency TODO
    writeControlCharacter(HEADER_BYTE);

    writeCharacter(iAnimation.numRows());
    writeCharacter(iAnimation.numColumns());
    writeCharacter(iAnimation.numFrames());

    try {
        for (int frame = 0; frame < iAnimation.numFrames(); frame++) {
            for(int i = 0; i < iAnimation.numRows(); i ++) {
                for(int j = 0; j < iAnimation.numColumns(); j++) {
                    writeColour(iAnimation.ledAt(i, j).frameAt(frame + INITIAL_FRAME).colour());
                }
            }
        }
    } catch (InvalidStateException* e) {
        // WTF?
    }

    writeControlCharacter(TERMINATING_BYTE);
}

void LedAnimCodec::readAnimation() {
    //add frame frequency TODO
    if (readCharacter().charValue() != HEADER_BYTE) {
        throw new InvalidAnimationException("No header byte");
    }

    int ledRows = readCharacter().intValue();
    int ledColumns = readCharacter().intValue();
    int numFrames = readCharacter().intValue();

    iAnimation.setupNew(ledRows, ledColumns, numFrames, DEFAULT_FRAME_FREQUENCY);

    int ledNum = 0;

    for(int frame = 0; frame < numFrames; frame++) {
        ledNum = 0;

        for(int row = 0; row < ledRows; row++) {
            for(int column = 0; column < ledColumns; column++) {
                iAnimation.ledAt(row, column).frameAt(frame + INITIAL_FRAME).setColour(readColour());
            }
        }
    }

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
    //return AnimChar();
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

const QString LedAnimStringCodec::asString() const {
    return iString;
}

const QByteArray& LedAnimStringCodec::asByteArray() const {
    //return iByteArray;
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

const QString LedAnimByteArrayCodec::asString() const {
    return "";
}

const QByteArray &LedAnimByteArrayCodec::asByteArray() const {
    return iByteArray;
}

