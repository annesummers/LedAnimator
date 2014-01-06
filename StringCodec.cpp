#include "StringCodec.h"

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

LedAnimStringCodec::LedAnimStringCodec(Animation &animation) :
    LedAnimCodec(animation),
    iLastReturn(0) {
}

const AnimChar LedAnimStringCodec::readCharacter() const {
    return AnimChar();
}

void LedAnimStringCodec::writeCharacter(AnimChar character) {
    unsigned char value = character.unsignedCharValue();

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
    int value = character.intValue();

    if(value != TERMINATING_BYTE &&
       value != HEADER_BYTE ) {
        throw IllegalArgumentException("LedAnimStringCodec::writeControlCharacter : not a control character");
    }

    iString.append(QString::number(character.intValue()));

    if(value == HEADER_BYTE) {
        iString.append(",");
    }
}

void LedAnimStringCodec::writeColour(Frame &frame) {
    const ColourValue& colourValue = static_cast<const ColourValue&>(frame.value());

    writeColour(colourValue.colour());
}

void LedAnimStringCodec::writeColour(QColor colour) {
    writeCharacter(colour.red());
    writeCharacter(colour.green());
    writeCharacter(colour.blue());
}

const QColor LedAnimStringCodec::readColour() const {
    return QColor();
}

void LedAnimStringCodec::writeFunction(Frame& frame) {
    const FunctionValue& value = static_cast<const FunctionValue&>(frame.value());

    writeCharacter(value.number());
}

const FunctionValue& LedAnimStringCodec::readFunction(Frame& frame) const {
    Q_UNUSED(frame);
}

void LedAnimStringCodec::writeFunctionData(Function function) {
    float multiplier = 256 * 256;

    float redIncrementF = (float)function.redIncrement();
    double redIncrementD = (double)redIncrementF;
    redIncrementD *= multiplier;
    redIncrementD = round(redIncrementD);

    if(redIncrementD > 0) {
        redIncrementD--;
    } else if(redIncrementD < 0) {
        redIncrementD++;
    }

    int redIncrement = (int)redIncrementD;

    unsigned char redIncrement1 = redIncrement;
    unsigned char redIncrement2 = redIncrement >> 8;
    unsigned char redIncrement3 = redIncrement >> 16;
    unsigned char redIncrement4 = redIncrement >> 24;
    writeCharacter(redIncrement1);
    writeCharacter(redIncrement2);
    writeCharacter(redIncrement3);
    writeCharacter(redIncrement4);

    float greenIncrementF = (float)function.greenIncrement();
    double greenIncrementD = (double)greenIncrementF;
    greenIncrementD *= multiplier;
    greenIncrementD = round(greenIncrementD);

    if(greenIncrementD > 0) {
        greenIncrementD--;
    } else if(greenIncrementD < 0) {
        greenIncrementD++;
    }

    int greenIncrement = (int)greenIncrementD;

    unsigned char greenIncrement1 = greenIncrement;
    unsigned char greenIncrement2 = greenIncrement >> 8;
    unsigned char greenIncrement3 = greenIncrement >> 16;
    unsigned char greenIncrement4 = greenIncrement >> 24;
    writeCharacter(greenIncrement1);
    writeCharacter(greenIncrement2);
    writeCharacter(greenIncrement3);
    writeCharacter(greenIncrement4);

    float blueIncrementF = (float)function.blueIncrement();
    double blueIncrementD = (double)blueIncrementF;
    blueIncrementD *= multiplier;
    blueIncrementD = round(blueIncrementD);

    if(blueIncrementD > 0) {
        blueIncrementD--;
    } else if(blueIncrementD < 0) {
        blueIncrementD++;
    }

    int blueIncrement = (int)blueIncrementD;

    unsigned char blueIncrement1 = blueIncrement;
    unsigned char blueIncrement2 = blueIncrement >> 8;
    unsigned char blueIncrement3 = blueIncrement >> 16;
    unsigned char blueIncrement4 = blueIncrement >> 24;
    writeCharacter(blueIncrement1);
    writeCharacter(blueIncrement2);
    writeCharacter(blueIncrement3);
    writeCharacter(blueIncrement4);
}

Function LedAnimStringCodec::readFunctionData() const {
    return Function();
}


const QString LedAnimStringCodec::asString() const {
    return iString;
}

const QByteArray& LedAnimStringCodec::asByteArray() const {
    return QByteArray();
}
