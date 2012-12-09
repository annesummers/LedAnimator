#include "ledanimcodec.h"

#include "led.h"
#include "frame.h"
#include "animation.h"

#include "defaults.h"
#include "exceptions.h"

AnimChar::AnimChar() {}

AnimChar::AnimChar(int value) :
    iValue(value) { }

AnimChar::AnimChar(unsigned char value) :
    iValue(value) { }

int AnimChar::intValue() {
    return static_cast<int>(iValue);
}

unsigned char AnimChar::charValue() {
    return iValue;
}

LedAnimCodec::LedAnimCodec(Animation& animation) :
    iAnimation(animation) {
}

void LedAnimCodec::writeAnimation() {
    writeCharacter(HEADER_BYTE);

    writeCharacter(iAnimation.numRows());
    writeCharacter(iAnimation.numColumns());
    writeCharacter(iAnimation.numFrames());

    try {
        for (int frame = 0; frame < iAnimation.numFrames(); frame++) {
            for(int i = 0; i < iAnimation.numRows(); i ++) {
                for(int j = 0; j < iAnimation.numColumns(); j++) {
                    writeColour(iAnimation.ledAt(i, j).colourAt(frame));
                }
            }
        }
    } catch (InvalidStateException* e) {
        // WTF?
    }

    writeCharacter(TERMINATING_BYTE);

    iAnimation.setSaved(true);
}

void LedAnimCodec::readAnimation() {
    if (readCharacter().charValue() != HEADER_BYTE) {
        throw new InvalidAnimationException("No header byte");
    }

    int ledRows = readCharacter().intValue();
    int ledColumns = readCharacter().intValue();
    int numFrames = readCharacter().intValue();

    iAnimation.setupNew(ledRows, ledColumns, numFrames);

    int ledNum = 0;

    for(int frame = 0; frame < numFrames; frame++) {
        ledNum = 0;

        for(int row = 0; row < ledRows; row++) {
            for(int column = 0; column < ledColumns; column++) {
                iAnimation.ledAt(row, column).setColourAt(frame, readColour());
            }
        }
    }

    if (readCharacter().charValue() != TERMINATING_BYTE) {
        throw new InvalidAnimationException("No terminating byte");
    }

    iAnimation.setSaved(true);
}

void LedAnimCodec::writeColour(QColor colour) {
    writeCharacter(colour.red());
    writeCharacter(colour.green());
    writeCharacter(colour.blue());
}

QColor LedAnimCodec::readColour() {
    int red = readCharacter().intValue();
    int green = readCharacter().intValue();
    int blue = readCharacter().intValue();

    return QColor::fromRgb(red, green, blue);
}

LedAnimStringCodec::LedAnimStringCodec(Animation &animation) :
    LedAnimCodec(animation) {
}

AnimChar LedAnimStringCodec::readCharacter() {
    return AnimChar();
}

void LedAnimStringCodec::writeCharacter(AnimChar character) {

}

QString LedAnimStringCodec::asString() {
    return iString;
}

QByteArray& LedAnimStringCodec::asByteArray() {
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

AnimChar LedAnimByteArrayCodec::readCharacter() {
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


QString LedAnimByteArrayCodec::asString() {
    return "";
}

QByteArray& LedAnimByteArrayCodec::asByteArray() {
    return iByteArray;
}

