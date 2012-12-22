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
    AnimatorBase(&animation, animation) {
}

void LedAnimCodec::writeAnimation() {
    writeCharacter(HEADER_BYTE);

    writeCharacter(animation().numRows());
    writeCharacter(animation().numColumns());
    writeCharacter(animation().numFrames());

    try {
        for (int frame = 0; frame < animation().numFrames(); frame++) {
            for(int i = 0; i < animation().numRows(); i ++) {
                for(int j = 0; j < animation().numColumns(); j++) {
                    writeColour(animation().ledAt(i, j).frameAt(frame + INITIAL_FRAME).colour());
                }
            }
        }
    } catch (InvalidStateException* e) {
        // WTF?
    }

    writeCharacter(TERMINATING_BYTE);

    animation().setSaved(true);
}

void LedAnimCodec::readAnimation() {
    if (readCharacter().charValue() != HEADER_BYTE) {
        throw new InvalidAnimationException("No header byte");
    }

    int ledRows = readCharacter().intValue();
    int ledColumns = readCharacter().intValue();
    int numFrames = readCharacter().intValue();

    animation().setupNew(ledRows, ledColumns, numFrames);

    int ledNum = 0;

    for(int frame = 0; frame < numFrames; frame++) {
        ledNum = 0;

        for(int row = 0; row < ledRows; row++) {
            for(int column = 0; column < ledColumns; column++) {
                animation().ledAt(row, column).frameAt(frame + INITIAL_FRAME).setColour(readColour());
            }
        }
    }

    if (readCharacter().charValue() != TERMINATING_BYTE) {
        throw new InvalidAnimationException("No terminating byte");
    }

    animation().setSaved(true);
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

LedAnimStringCodec::LedAnimStringCodec(Animation &animation) :
    LedAnimCodec(animation) {
}

const AnimChar LedAnimStringCodec::readCharacter() const {
    //return AnimChar();
}

void LedAnimStringCodec::writeCharacter(AnimChar character) {
    Q_UNUSED(character);
}

const QString LedAnimStringCodec::asString() const {
    return iString;
}

const QByteArray& LedAnimStringCodec::asByteArray() const {
    //return iByteArray;
}

LedAnimByteArrayCodec::LedAnimByteArrayCodec(Animation &animation) :
    LedAnimCodec(animation),
    iPosition(0) {}


LedAnimByteArrayCodec::LedAnimByteArrayCodec(Animation& animation, QByteArray bytes) :
    LedAnimCodec(animation),
    iByteArray(bytes),
    iPosition(0) {
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


const QString LedAnimByteArrayCodec::asString() const {
    return "";
}

const QByteArray &LedAnimByteArrayCodec::asByteArray() const {
    return iByteArray;
}

