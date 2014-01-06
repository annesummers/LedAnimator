#include "ByteArrayCodec.h"

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
    char value = character.unsignedCharValue();

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
    char value = character.unsignedCharValue();
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

    unsigned int rows = readCharacter().unsignedIntValue();
    unsigned int columns = readCharacter().unsignedIntValue();

    for(int i = 0; i < rows * columns; i++) {
        positions.append(INVALID);
    }

    unsigned int ledNum;
    unsigned int row;
    unsigned int column;

    for(int i = 0; i < numLeds; i++) {
        ledNum = readCharacter().unsignedCharValue();

        //unsigned char ledNumHigh = readCharacter().unsignedCharValue();
       // unsigned char ledNumLow = readCharacter().unsignedCharValue();

        //ledNum =  ledNumHigh |= ledNumLow << 8;
        row = readCharacter().unsignedCharValue();
        column = readCharacter().unsignedCharValue();

        if(row != INVALID && column != INVALID) {
            positions.replace((row*columns) + column, ledNum);
        }
    }

    *numRows = rows;
    *numColumns = columns;

    return positions;
}

void LedAnimByteArrayCodec::writeColour(Frame &frame) {
    const ColourValue& colourValue = static_cast<const ColourValue&>(frame.value());

    writeColour(colourValue.colour());
}

void LedAnimByteArrayCodec::writeColour(QColor colour) {
    int hue = colour.hue();
    char hueHigh = hue;
    char hueLow = hue >> 8;
    writeCharacter(hueHigh);
    writeCharacter(hueLow);
    writeCharacter(colour.saturation());
    writeCharacter(colour.value());
}

const QColor LedAnimByteArrayCodec::readColour() const {
    unsigned char hueHigh = readCharacter().unsignedCharValue();
    char hueLow = readCharacter().unsignedCharValue();

    int hue = hueHigh;
    hue |= hueLow << 8;

    unsigned char saturation = readCharacter().unsignedCharValue();
    unsigned char value = readCharacter().unsignedCharValue();

    QColor colour = QColor::fromHsv(hue, saturation, value);

    if(!colour.isValid()) {
        qDebug("invalid colour");
    }

    return colour;
}

void LedAnimByteArrayCodec::writeFunction(Frame& frame) {
    const FunctionValue& value = static_cast<const FunctionValue&>(frame.value());

    writeFunctionData(value.function());
    writeCharacter(value.number());
}

const FunctionValue& LedAnimByteArrayCodec::readFunction(Frame& frame) const {
    Function function = readFunctionData();

    readCharacter().intValue();
    return *(new FunctionValue(&frame,
                            function ,iAnimation.addFunction(function)));
                    //));
}

void LedAnimByteArrayCodec::writeFunctionData(Function function) {
    float redIncrement = (float)function.redIncrement();
    unsigned char *p = reinterpret_cast<unsigned char *>(&redIncrement);
    for (std::size_t i = 0; i != sizeof(float); ++i) {
        writeCharacter(p[i]);
    }

    float greenIncrement = (float)function.greenIncrement();
    p = reinterpret_cast<unsigned char *>(&greenIncrement);
    for (std::size_t i = 0; i != sizeof(float); ++i) {
        writeCharacter(p[i]);
    }

    float blueIncrement = (float)function.blueIncrement();
    p = reinterpret_cast<unsigned char *>(&blueIncrement);
    for (std::size_t i = 0; i != sizeof(float); ++i) {
        writeCharacter(p[i]);
    }
}

Function LedAnimByteArrayCodec::readFunctionData() const{
    float redIncrement = 0;
    unsigned char *p = reinterpret_cast<unsigned char *>(&redIncrement);
    for (std::size_t i = 0; i != sizeof(float); ++i) {
        *(p+i) = readCharacter().unsignedCharValue();
    }

    float greenIncrement = 0;
    p = reinterpret_cast<unsigned char *>(&greenIncrement);
    for (std::size_t i = 0; i != sizeof(float); ++i) {
        *(p+i) = readCharacter().unsignedCharValue();
    }

    float blueIncrement = 0;
    p = reinterpret_cast<unsigned char *>(&blueIncrement);
    for (std::size_t i = 0; i != sizeof(float); ++i) {
        *(p+i) = readCharacter().unsignedCharValue();
    }

    Function function = Function(redIncrement,
                                 greenIncrement,
                                blueIncrement);

    return function;
}

const QString LedAnimByteArrayCodec::asString() const {
    return "";
}

const QByteArray &LedAnimByteArrayCodec::asByteArray() const {
    return iByteArray;
}

LedAnimSimpleByteArrayCodec::LedAnimSimpleByteArrayCodec(Animation &animation) :
    LedAnimByteArrayCodec(animation) {}


LedAnimSimpleByteArrayCodec::LedAnimSimpleByteArrayCodec(Animation& animation, QByteArray bytes) :
    LedAnimByteArrayCodec(animation, bytes) {
}


void LedAnimSimpleByteArrayCodec::writeColour(Frame &frame) {
    const ColourValue& colourValue = static_cast<const ColourValue&>(frame.value());

    writeColour(colourValue.colour());
}

void LedAnimSimpleByteArrayCodec::writeColour(QColor colour) {
    writeCharacter(colour.red());
    writeCharacter(colour.green());
    writeCharacter(colour.blue());
}

const QColor LedAnimSimpleByteArrayCodec::readColour() const {
    return QColor();
}

void LedAnimSimpleByteArrayCodec::writeFunction(Frame& frame) {
    const FunctionValue& value = static_cast<const FunctionValue&>(frame.value());

    writeCharacter(value.number());
}

const FunctionValue& LedAnimSimpleByteArrayCodec::readFunction(Frame& frame) const {
    Q_UNUSED(frame);
}

void LedAnimSimpleByteArrayCodec::writeFunctionData(Function function) {
    float multiplier = 256 * 256;

    float redIncrementF = (float)function.redIncrement();
    double redIncrementD = (double)redIncrementF;
    redIncrementD *= multiplier;
    redIncrementD = round(redIncrementD);
    int redIncrement = (int)redIncrementD;

    char redIncrementHigh = redIncrement;
    char redIncrementLow = redIncrement >> 8;
    writeCharacter(redIncrementHigh);
    writeCharacter(redIncrementLow);

    float greenIncrementF = (float)function.greenIncrement();
    double greenIncrementD = (double)greenIncrementF;
    greenIncrementD *= multiplier;
    greenIncrementD = round(greenIncrementD);
    int greenIncrement = (int)greenIncrementD;

    char greenIncrementHigh = greenIncrement;
    char greenIncrementLow = greenIncrement >> 8;
    writeCharacter(greenIncrementHigh);
    writeCharacter(greenIncrementLow);

    float blueIncrementF = (float)function.blueIncrement();
    double blueIncrementD = (double)blueIncrementF;
    blueIncrementD *= multiplier;
    blueIncrementD = round(blueIncrementD);
    int blueIncrement = (int)blueIncrementD;

    char blueIncrementHigh = blueIncrement;
    char blueIncrementLow = blueIncrement >> 8;
    writeCharacter(blueIncrementHigh);
    writeCharacter(blueIncrementLow);
}

Function LedAnimSimpleByteArrayCodec::readFunctionData() const {
    return Function();
}
